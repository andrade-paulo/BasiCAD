#include "BasiCAD.h"


void showInstructions() {
    std::string helpText =
        "--- CONTROLES GERAIS ---\n"
        "H: Mostra esta janela de ajuda.\n"
        "V: Alterna entre visão única e múltipla.\n"
        "ESC: Fecha a aplicação.\n"
        "\n"
        "--- CRIAÇÃO DE OBJETOS ---\n"
        "B: Adiciona um Cubo (Box).\n"
        "C: Adiciona um Cilindro.\n"
        "S: Adiciona uma Esfera.\n"
        "P: Adiciona uma Grade (Grid).\n"
        "G: Adiciona uma Geo-Esfera.\n"
        "1 - 5: Carrega modelos .obj da pasta Resources.\n"
        "\n"
        "--- MANIPULAÇÃO DE OBJETOS ---\n"
        "TAB: Seleciona o próximo objeto.\n"
        "DELETE: Remove o objeto selecionado.\n"
        "\n"
        "--- TRANSFORMAÇÕES ---\n\n"
        "MOVER (TRANSLATE):\n"
        "Setas Cima/Baixo: Move na profundidade (Z).\n"
        "Setas Esq/Dir: Move lateralmente (X).\n"
        "[: Move para cima (Y).\n"
        "]: Move para baixo (Y).\n"
        "\n"
        "GIRAR (ROTATE):\n"
        "SHIFT + Setas Cima/Baixo: Gira no eixo X.\n"
        "SHIFT + Setas Esq/Dir: Gira no eixo Y.\n"
        "SHIFT + [: Gira no eixo Z (sentido horário).\n"
        "SHIFT + ]: Gira no eixo Z (sentido anti-horário).\n"
        "\n"
        "ESCALA (SCALE):\n"
        "+: Aumenta o tamanho do objeto.\n"
        "-: Diminui o tamanho do objeto.\n";

    MessageBox(nullptr, helpText.c_str(), "Comandos - BasiCAD", MB_OK | MB_ICONINFORMATION);
}


void BasiCAD::processInput() {
    // Mapeamento das teclas de comando
    if (input->KeyPress(VK_ESCAPE))
        window->Close();

	if (input->KeyPress('H')) {
		showInstructions(); // exibe a janela de ajuda
	}

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

    if (input->KeyPress('1'))
        loadObjFile("Resources/ball.obj");
	if (input->KeyPress('2'))
		loadObjFile("Resources/capsule.obj");
	if (input->KeyPress('3'))
		loadObjFile("Resources/house.obj");
	if (input->KeyPress('4'))
		loadObjFile("Resources/monkey.obj");
	if (input->KeyPress('5'))
		loadObjFile("Resources/thorus.obj");
	if (input->KeyPress('6'))
		loadObjFile("Resources/pegazuls.obj");

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
