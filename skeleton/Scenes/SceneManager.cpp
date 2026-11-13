#include "SceneManager.h"
#include <iostream>
#include "imgui.h"

// Solicita el cambio de escena. Si el nombre no está registrado, se escribe
// un error en stderr y la solicitud se ignora.
void SceneManager::changeScene(const std::string& name) {
    if (m_factories.find(name) == m_factories.end()) {
        std::cerr << "[SceneManager Error] Escena no registrada: " << name << "\n";
        return;
    }
    m_pendingSceneName = name;
    m_hasPendingChange = true;
}

// Realiza la transición a la escena pendiente: limpia la escena actual,
// crea la nueva usando la fábrica y la inicializa.
void SceneManager::applyPendingSceneChange() {
    if (!m_hasPendingChange) return;

    if (m_currentScene) {
        m_currentScene->cleanup();
    }

    m_currentScene = m_factories[m_pendingSceneName]();
    if (m_currentScene) {
        m_currentScene->init();
        std::cout << "[SceneManager] Escena cargada: " << m_pendingSceneName << "\n";
    }

    m_hasPendingChange = false;
    m_pendingSceneName.clear();
}

// Llamada por frame: aplica el cambio pendiente antes de actualizar la escena
// para evitar modificar la escena mientras está siendo usada.
void SceneManager::update(double dt) {
    if (m_hasPendingChange) {
        applyPendingSceneChange();
    }

    if (m_currentScene) {
        m_currentScene->update(dt);
    }
}

void SceneManager::shutdown() {
    if (m_currentScene) {
        m_currentScene->cleanup();
        m_currentScene.reset();
    }

    m_hasPendingChange = false;
    m_pendingSceneName.clear();
}

// Reenvía eventos de teclado a la escena actual. Mantener este método simple
// permite centralizar atajos globales si se desea en el futuro.
void SceneManager::keyPress(unsigned char key, const physx::PxTransform& cameraTransform) {
    // Teclas globales de navegación entre prácticas
    if (key == '0') {
        changeScene("P0S_Scene");
        return; // Consumimos el evento para que no interfiera con la escena
    }
    if (key == '1') {
        changeScene("P1S_Scene");
        return; // Consumimos el evento para que no interfiera con la escena
    }
    if (key == '2') {
        changeScene("P2S_Scene");
        return; // Consumimos el evento para que no interfiera con la escena
    }
    if (key == '3') {
        changeScene("P3S_Scene");
        return; // Consumimos el evento para que no interfiera con la escena
    }
    if (key == '4') {
        changeScene("P4S_Scene");
        return; // Consumimos el evento para que no interfiera con la escena
    }
    if (key == '5') {
        changeScene("EscenaProyectoIntermedio");
        return; // Consumimos el evento para que no interfiera con la escena
    }

    // Si no es una tecla de navegación global, se la pasamos a la escena activa
    if (m_currentScene != nullptr) {
        m_currentScene->keyPress(key, cameraTransform);
    }
}

void SceneManager::drawDebugUI() {
    const std::string current = m_currentScene ? m_currentScene->getName() : "(ninguna)";

    if (ImGui::BeginCombo("Escena", current.c_str())) {
        for (const auto& entry : m_factories) {
            const std::string& name = entry.first;
            if (ImGui::Selectable(name.c_str(), name == current))
                changeScene(name);          // diferido: se aplica en el siguiente update()
        }
        ImGui::EndCombo();
    }
    ImGui::SameLine();
    if (ImGui::Button("Reiniciar") && m_currentScene)
        changeScene(m_currentScene->getName());

    ImGui::Separator();

    if (m_currentScene)                     // en el primer frame aún no existe
        m_currentScene->drawDebugUI();
}