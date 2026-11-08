#include <vector>

#include "PxPhysicsAPI.h"

#include "core.hpp"
#include "RenderUtils.hpp"

#include "imgui.h"
#include "imgui_impl_glut.h"
#include "imgui_impl_opengl2.h"

using namespace physx;

extern void initPhysics(bool interactive);
extern void stepPhysics(bool interactive, double t);
extern void cleanupPhysics(bool interactive);
extern void keyPress(unsigned char key, const PxTransform& camera);
extern PxPhysics* gPhysics;
extern PxMaterial* gMaterial;

std::vector<const RenderItem*> gRenderItems;

double PCFreq = 0.0;
__int64 CounterStart = 0;
__int64 CounterLast = 0;

// Nota: (**) con eso se evita que una accion en la vent. de imgui afecte al comportamiento de nuestra escena.

void StartCounter()
{
	LARGE_INTEGER li;
	if (!QueryPerformanceFrequency(&li))
		return;

	PCFreq = double(li.QuadPart) /*/ 1000.0*/;

	QueryPerformanceCounter(&li);
	CounterStart = li.QuadPart;
	CounterLast = CounterStart;
}

double GetCounter()
{
	LARGE_INTEGER li;
	QueryPerformanceCounter(&li);
	double t = double(li.QuadPart - CounterLast) / PCFreq;
	CounterLast = li.QuadPart;
	return t;
}

namespace
{
	Camera* sCamera;

	void motionCallback(int x, int y)
	{
		ImGui_ImplGLUT_MotionFunc(x, y);

		if (!ImGui::GetIO().WantCaptureMouse) // ** para evitar que la camara reciba eventos utilizados por imgui
			sCamera->handleMotion(x, y);
	}

	//La sobreescribo aqui para no tocar el Render.cpp
	void reshapeCallback(int width, int height) {
		ImGui_ImplGLUT_ReshapeFunc(width, height); //Con esto se inicializa el display size
		glViewport(0, 0, width, height);
	}

	void keyboardCallback(unsigned char key, int x, int y)
	{
		ImGui_ImplGLUT_KeyboardFunc(key, x, y); //conviven los callbacks de imgui con los de glut

		if (key == 27)
			exit(0);

		if (!ImGui::GetIO().WantCaptureKeyboard) //** 
		{
			if (!sCamera->handleKey(key, x, y))
				keyPress(key, sCamera->getTransform());
		}
	}

	void mouseCallback(int button, int state, int x, int y)
	{
		ImGui_ImplGLUT_MouseFunc(button, state, x, y);

		if (!ImGui::GetIO().WantCaptureMouse) //** 
			sCamera->handleMouse(button, state, x, y);
	}

	void passiveMotionCallback(int x, int y) //asi recibe imgui el mov del raton aunque no se pulse/mueva nada
	{
		ImGui_ImplGLUT_MotionFunc(x, y);
	}

	void idleCallback()
	{
		glutPostRedisplay();
	}

	float stepTime = 0.0f;
	//#define FIXED_STEP

	void renderCallback()
	{
		double t = GetCounter();
#ifdef FIXED_STEP
		if (t < (1.0f / 30.0f))
		{
			fprintf(stderr, "Time: %f\n", stepTime);
			stepTime += t;
		}
		else
			stepTime = 1.0f / 30.0f;

		if (stepTime >= (1.0f / 30.0f))
		{
			stepPhysics(true, stepTime);
			stepTime = 0.0f;
		}
#else
		stepPhysics(true, t);
#endif

		//Logica integrada segun el ejemplo de implGlut para ImGui
		// 1. Preparar un nuevo frame de ImGui
		ImGui_ImplOpenGL2_NewFrame();
		ImGui_ImplGLUT_NewFrame();
		ImGui::NewFrame();

		// 2. Construir la interfaz
		ImGui::ShowDemoWindow();

		//Escena 3D
		startRender(sCamera->getEye(), sCamera->getDir());

		//fprintf(stderr, "Num Render Items: %d\n", static_cast<int>(gRenderItems.size()));
		for (auto it = gRenderItems.begin(); it != gRenderItems.end(); ++it)
		{
			const RenderItem* obj = (*it);
			auto objTransform = obj->transform;
			if (!objTransform)
			{
				auto actor = obj->actor;
				if (actor)
				{
					renderShape(*obj->shape, actor->getGlobalPose(), obj->color);
					continue;
				}
			}
			renderShape(*obj->shape, objTransform ? *objTransform : physx::PxTransform(PxIdentity), obj->color);
		}

		//PxScene* scene;
		//PxGetPhysics().getScenes(&scene, 1);
		//PxU32 nbActors = scene->getNbActors(PxActorTypeFlag::eRIGID_DYNAMIC | PxActorTypeFlag::eRIGID_STATIC);
		//if (nbActors)
		//{
		//	std::vector<PxRigidActor*> actors(nbActors);
		//	scene->getActors(PxActorTypeFlag::eRIGID_DYNAMIC | PxActorTypeFlag::eRIGID_STATIC, reinterpret_cast<PxActor**>(&actors[0]), nbActors);
		//	renderActors(&actors[0], static_cast<PxU32>(actors.size()), true, Vector4(1.0f, 0.0f, 0.0f, 1.0f));
		//}

		// 4. Renderizar ImGui sobre la escena
		ImGui::Render();
		ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());

		finishRender(); //intercambia los buffers (evita el flickering)
	}

	void exitCallback(void)
	{
		//Shutdown de lo inicializado
		ImGui_ImplGLUT_Shutdown();
		ImGui_ImplOpenGL2_Shutdown();
		ImGui::DestroyContext();

		delete sCamera;
		cleanupPhysics(true);
	}
}

void renderLoop()
{
	StartCounter();
	sCamera = new Camera(PxVec3(60.0f, 40.0f, 50.0f), PxVec3(-0.6f, -0.2f, -0.7f));

	setupDefaultWindow("Simulacion Fisica Videojuegos");
	setupDefaultRenderState();

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplGLUT_Init();
	ImGui_ImplOpenGL2_Init();

	glutIdleFunc(idleCallback);
	glutDisplayFunc(renderCallback);
	glutKeyboardFunc(keyboardCallback);
	glutMouseFunc(mouseCallback);
	glutMotionFunc(motionCallback);
	motionCallback(0, 0);
	glutReshapeFunc(reshapeCallback); //Registramos la func de ESTA clase
	glutPassiveMotionFunc(passiveMotionCallback); //

	initPhysics(true);
	atexit(exitCallback); //despues de la fisica para que se ejecute antes de la destrucción estática de SceneManager.
	glutMainLoop();
}

void RegisterRenderItem(const RenderItem* _item)
{
	gRenderItems.push_back(_item);
}

void DeregisterRenderItem(const RenderItem* _item)
{
	auto it = find(gRenderItems.begin(), gRenderItems.end(), _item);
	gRenderItems.erase(it);
}

double GetLastTime()
{
	double t = double(CounterLast - CounterStart) / PCFreq;
	return t;
}

Camera* GetCamera()
{
	return sCamera;
}

PxShape* CreateShape(const PxGeometry& geo, const PxMaterial* mat)
{
	if (mat == nullptr)
		mat = gMaterial; // Default material

	PxShape* shape = gPhysics->createShape(geo, *mat);
	return shape;
}