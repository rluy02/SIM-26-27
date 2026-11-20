#include "WhirlwindGenerator.h"
#include "../ParticleWithForces.h"
#include <algorithm>
#include <cmath>
#include "imgui.h"

WhirlwindGenerator::WhirlwindGenerator(const Vector3D& pos, float rad, float rInfl, float vs, float k) : WindGenerator(Vector3D(0.f, 0.f, 0.f), rad),
_pos(pos), _radiusInfluence(rInfl), _verticalStop(vs), _k(k) {
	rebuildVisual();
}

WhirlwindGenerator::~WhirlwindGenerator()
{
	clearVisual();
}

void WhirlwindGenerator::clearVisual() {
	for (RenderItem*& item : _edgeItems) {
		if (item) item->release();
		item = nullptr;
	}
}

Vector3D WhirlwindGenerator::getWindVelocity(const ParticleWithForces* p) const {
	assert(p != nullptr);
	const Vector3D pPos = p->getPosition();

	const float dx = pPos.x - _pos.x;
	const float dy = _verticalStop - (pPos.y - _pos.y);
	const float dz = pPos.z - _pos.z;

	//Para representar mejor el area de efecto del debug
	if (std::abs(dx) > _radiusInfluence || std::abs(dz) > _radiusInfluence || dy < 0.f || dy > _verticalStop) {
		return Vector3D(0.f, 0.f, 0.f);
	}

	return _k * Vector3D(-dz, dy, dx);
}

void WhirlwindGenerator::drawUI() {
	float c[3] = { _pos.x, _pos.y, _pos.z };
	if (ImGui::DragFloat3("Posicion", c, 0.5f))
		setPosition(Vector3D(c[0], c[1], c[2])); //actualiza logica y graficamente

	bool changedR = ImGui::DragFloat("Radio de influencia", &_radiusInfluence, 0.5f, 0.5f, 300.f, "%.1f", ImGuiSliderFlags_AlwaysClamp);
	bool changedH = ImGui::DragFloat("Altura de parada", &_verticalStop, 0.5f, 0.5f, 300.f, "%.1f", ImGuiSliderFlags_AlwaysClamp);
	if (changedR || changedH) rebuildVisual();

	ImGui::DragFloat("K", &_k, 0.01f, 0.f, 10.f, "%.2f", ImGuiSliderFlags_AlwaysClamp);
}

void WhirlwindGenerator::setPosition(const Vector3D& pos) {
	_pos = pos;
	rebuildVisual(); //no basta con _transform.p: el centro visual esta desplazado
};

void WhirlwindGenerator::rebuildVisual() {
	clearVisual();
	if (!_visible) return;
	const float hx = (std::max)(_radiusInfluence, 0.01f);
	const float hy = (std::max)(_verticalStop * 0.5f, 0.01f);
	const float hz = hx;
	const float t = 0.1f; // semigrosor de las aristas
	const physx::PxVec3 c(_pos.x, _pos.y + hy, _pos.z); // centro de la caja (base en _pos.y)

	int i = 0;
	for (float a : { -1.f, 1.f }) {
		for (float b : { -1.f, 1.f }) {
			addEdge(i++, c + physx::PxVec3(a * hx, 0.f, b * hz), physx::PxVec3(t, hy, t));   // 4 aristas verticales
			addEdge(i++, c + physx::PxVec3(0.f, a * hy, b * hz), physx::PxVec3(hx, t, t));   // 4 aristas en X
			addEdge(i++, c + physx::PxVec3(a * hx, b * hy, 0.f), physx::PxVec3(t, t, hz));   // 4 aristas en Z
		}
	}
}

void WhirlwindGenerator::addEdge(int i, const physx::PxVec3& center, const physx::PxVec3& halfExtents) {
	_edgeTransforms[i] = physx::PxTransform(center);
	physx::PxShape* shape = CreateShape(physx::PxBoxGeometry(halfExtents));
	_edgeItems[i] = new RenderItem(shape, &_edgeTransforms[i], Vector4(0.2f, 0.6f, 1.f, 1.f));
	shape->release();
}

void WhirlwindGenerator::setGizmoVisible(bool v) {
	if (v == _visible) return;
	_visible = v;
	rebuildVisual();
}