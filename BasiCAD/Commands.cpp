#include "BasiCAD.h"


void BasiCAD::processInput() {
    // Mapeamento das teclas de comando

    if (input->KeyPress(VK_ESCAPE))
        window->Close();

    if (input->KeyPress('V'))
        multipleView = !multipleView; // alterna entre visualização única e múltipla

    if (input->KeyPress('B'))
        addBox();
    if (input->KeyPress('C'))
        addCylinder();
    if (input->KeyPress('S'))
        addSphere();
    if (input->KeyPress('P'))
        addGrid();
    if (input->KeyPress('G'))
        addObj(&geoSphere);

    if (input->KeyPress(VK_DELETE))
        removeObject();

    // Seleção
    if (input->KeyPress(VK_TAB)) {
        changeColor(selectedObject, DEFAULT_COLOR);  // objeto selecionado atual
        selectedObject = (selectedObject + 1) % scene.size();
        changeColor(selectedObject, SELECTED_COLOR);  // novo objeto selecionado
    }

    // Escala
    if (input->KeyPress(VK_ADD)) {  // tecla '+'
        auto& obj = scene[selectedObject];
        XMStoreFloat4x4(
            &obj.world,
            XMMatrixMultiply(XMLoadFloat4x4(&obj.world), XMMatrixScaling(1.1f, 1.1f, 1.1f))
        );
    }

    if (input->KeyPress(VK_SUBTRACT)) {  // tecla '-'
        auto& obj = scene[selectedObject];
        XMStoreFloat4x4(
            &obj.world,
            XMMatrixMultiply(XMLoadFloat4x4(&obj.world), XMMatrixScaling(0.9f, 0.9f, 0.9f))
        );
    }

    double timeDelta = gameTime * 30;

    if (input->KeyDown(VK_SHIFT)) {
        // Rotação
        auto& obj = scene[selectedObject];
        double angle = XMConvertToRadians(2.0f); // ângulo de rotação em radianos

        // Eixo X
        if (input->KeyDown(VK_LEFT)) {
            XMStoreFloat4x4(
                &obj.world,
                XMMatrixMultiply(XMLoadFloat4x4(&obj.world), XMMatrixRotationY(-angle * timeDelta))
            );
        }
        if (input->KeyDown(VK_UP)) {
            XMStoreFloat4x4(
                &obj.world,
                XMMatrixMultiply(XMLoadFloat4x4(&obj.world), XMMatrixRotationX(angle * timeDelta))
            );
        }

        // Eixo Y
        if (input->KeyDown(VK_RIGHT)) {
            XMStoreFloat4x4(
                &obj.world,
                XMMatrixMultiply(XMLoadFloat4x4(&obj.world), XMMatrixRotationY(angle * timeDelta))
            );
        }
        if (input->KeyDown(VK_DOWN)) {
            XMStoreFloat4x4(
                &obj.world,
                XMMatrixMultiply(XMLoadFloat4x4(&obj.world), XMMatrixRotationX(-angle * timeDelta))
            );
        }

        // Eixo Z
        if (input->KeyDown(VK_OEM_6)) {  // tecla '['
            XMStoreFloat4x4(
                &obj.world,
                XMMatrixMultiply(XMLoadFloat4x4(&obj.world), XMMatrixRotationZ(angle * timeDelta))
            );
        }
        if (input->KeyDown(VK_OEM_5)) {  // tecla ']'
            XMStoreFloat4x4(
                &obj.world,
                XMMatrixMultiply(XMLoadFloat4x4(&obj.world), XMMatrixRotationZ(-angle * timeDelta))
            );
        }
    }
    else {
        // Translação

        // Eixo X
        if (input->KeyDown(VK_RIGHT)) {
            auto& obj = scene[selectedObject];
            XMStoreFloat4x4(
                &obj.world,
                XMLoadFloat4x4(&obj.world) * XMMatrixTranslation(-MOVEMENT_PACE * timeDelta, 0.0f, 0.0f)
            );
        }

        if (input->KeyDown(VK_LEFT)) {
            auto& obj = scene[selectedObject];
            XMStoreFloat4x4(
                &obj.world,
                XMMatrixMultiply(XMLoadFloat4x4(&obj.world), XMMatrixTranslation(MOVEMENT_PACE * timeDelta, 0.0f, 0.0f))
            );
        }

        // Eixo Z
        if (input->KeyDown(VK_UP)) {
            auto& obj = scene[selectedObject];
            XMStoreFloat4x4(
                &obj.world,
                XMMatrixMultiply(XMLoadFloat4x4(&obj.world), XMMatrixTranslation(0.0f, 0.0f, -MOVEMENT_PACE * timeDelta))
            );
        }

        if (input->KeyDown(VK_DOWN)) {
            auto& obj = scene[selectedObject];
            XMStoreFloat4x4(
                &obj.world,
                XMMatrixMultiply(XMLoadFloat4x4(&obj.world), XMMatrixTranslation(0.0f, 0.0f, MOVEMENT_PACE * timeDelta))
            );
        }

        // Eixo Y
        if (input->KeyDown(VK_OEM_6)) {  // tecla '['
            auto& obj = scene[selectedObject];
            XMStoreFloat4x4(
                &obj.world,
                XMMatrixMultiply(XMLoadFloat4x4(&obj.world), XMMatrixTranslation(0.0f, MOVEMENT_PACE * timeDelta, 0.0f))
            );
        }

        if (input->KeyDown(VK_OEM_5)) {  // tecla ']'
            auto& obj = scene[selectedObject];
            XMStoreFloat4x4(
                &obj.world,
                XMMatrixMultiply(XMLoadFloat4x4(&obj.world), XMMatrixTranslation(0.0f, -MOVEMENT_PACE * timeDelta, 0.0f))
            );
        }
    }
}
