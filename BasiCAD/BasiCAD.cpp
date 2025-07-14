/**********************************************************************************
// BasiCAD (Código Fonte)
//
// Descrição: Sistema de CAD básico com geometrias 3D
//
**********************************************************************************/

#include "BasiCAD.h"

// ------------------------------------------------------------------------------

Timer BasiCAD::timer;

// ------------------------------------------------------------------------------

void BasiCAD::Init()
{
    // --------------------------------------
    // Transformação, Visualização e Projeção
    // --------------------------------------

    // posição inicial da câmera
    camera = { XM_PIDIV2, XM_PIDIV4, 5.0f };

    // inicializa a matriz de projeção
    XMStoreFloat4x4(&Proj, XMMatrixPerspectiveFovLH(
        XMConvertToRadians(45.0f), 
        window->AspectRatio(), 
        1.0f, 100.0f));

    // -------------------------
    // Definição dos Objetos 3D
    // -------------------------

    // grid
    Grid* startGridPtr = &grid;
	addObj(startGridPtr);
	
    auto& gridObj = scene[0];
    XMStoreFloat4x4(
        &gridObj.world,
        XMMatrixMultiply(XMLoadFloat4x4(&gridObj.world), 
        XMMatrixScaling(3.0f, 3.0f, 3.0f) * XMMatrixTranslation(0.0f, 0.0f, 0.0f))
    );

    // ------------------
    // Viewports
    // ------------------

    // frente
    viewFront.TopLeftX = 0.0f;
    viewFront.TopLeftY = 0.0f;
    viewFront.Width = float(window->Width() / 2.0f);
    viewFront.Height = float(window->Height() / 2.0f);
    viewFront.MinDepth = 0.0f;
    viewFront.MaxDepth = 1.0f;

    // lado direito
    viewRight.TopLeftX = float(window->Width() / 2.0f);
    viewRight.TopLeftY = 0.0f;
    viewRight.Width = float(window->Width() / 2.0f);
    viewRight.Height = float(window->Height() / 2.0f);
    viewRight.MinDepth = 0.0f;
    viewRight.MaxDepth = 1.0f;

	// topo
	viewTop.TopLeftX = 0.0f;
	viewTop.TopLeftY = float(window->Height() / 2.0f);
	viewTop.Width = float(window->Width() / 2.0f);
	viewTop.Height = float(window->Height() / 2.0f);
	viewTop.MinDepth = 0.0f;
	viewTop.MaxDepth = 1.0f;

	// perspectiva
	viewPerspective.TopLeftX = float(window->Width() / 2.0f);
	viewPerspective.TopLeftY = float(window->Height() / 2.0f);
	viewPerspective.Width = float(window->Width() / 2.0f);
	viewPerspective.Height = float(window->Height() / 2.0f);
	viewPerspective.MinDepth = 0.0f;
	viewPerspective.MaxDepth = 1.0f;
 
    // ---------------------

    BuildRootSignature();
    BuildPipelineState();    

    // ----------------------

    timer.Start();
}

// ------------------------------------------------------------------------------

void BasiCAD::Update()
{
    // Mapeamento das teclas de comando

    if (input->KeyPress(VK_ESCAPE))
        window->Close();

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
		float angle = XMConvertToRadians(2.0f); // ângulo de rotação em radianos

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


    // atualiza posição da câmera
    camera.Update();

    // constrói a matriz de visualização
    XMVECTOR pos = XMVectorSet(camera.x, camera.y, camera.z, 1.0f);
    XMVECTOR target = XMVectorZero();
    XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
    XMMATRIX view = XMMatrixLookAtLH(pos, target, up);

    // carrega matriz de projeção
    XMMATRIX proj = XMLoadFloat4x4(&Proj);

    // ajusta o constant buffer de cada objeto
    for (auto & obj : scene)
    {
        // carrega matriz de mundo
        XMMATRIX world = XMLoadFloat4x4(&obj.world);      

        // constrói matriz combinada
        XMMATRIX WorldViewProj = world * view * proj;        

        // atualiza o buffer constante com a matriz combinada
        Constants constants;
        XMStoreFloat4x4(&constants.WorldViewProj, XMMatrixTranspose(WorldViewProj));
        obj.cbuffer->Copy(&constants);
    }
}

// ------------------------------------------------------------------------------

void BasiCAD::Draw()
{
    // limpa o backbuffer
    graphics->Clear();

    // comandos de configuração comuns a todos os objetos
    graphics->CommandList()->SetPipelineState(pipelineState);
    graphics->CommandList()->SetGraphicsRootSignature(rootSignature);
    graphics->CommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    
    // desenha objetos da cena
    for (auto& obj : scene)
    {
        // comandos de configuração específicos a cada objeto
        graphics->CommandList()->SetGraphicsRootConstantBufferView(0, obj.cbuffer->View());
        graphics->CommandList()->IASetVertexBuffers(0, 1, obj.vbuffer->View());
        graphics->CommandList()->IASetIndexBuffer(obj.ibuffer->View());

        // desenha objeto
        graphics->CommandList()->DrawIndexedInstanced(
            obj.mesh->indexCount, 1,
            obj.mesh->startIndex,
            obj.mesh->baseVertex,
            0);
    }
    
    // apresenta o backbuffer na tela
    graphics->Present();    
}

// ------------------------------------------------------------------------------

void BasiCAD::Finalize()
{
    // espera GPU finalizar comandos pendentes
    graphics->WaitForGpu();

    // libera memória alocada
    rootSignature->Release();
    pipelineState->Release();

    for (auto& obj : scene)
    {
        delete obj.mesh;
        delete obj.vbuffer;
        delete obj.ibuffer;
        delete obj.cbuffer;
    }
}

// ------------------------------------------------------------------------------
void BasiCAD::addObj(Geometry* geo) {
	Object obj;

	XMStoreFloat4x4(&obj.world, XMMatrixIdentity());

	obj.mesh = new Mesh(*geo);
    obj.vbuffer = new VertexBuffer<Vertex>(*geo);
    obj.ibuffer = new IndexBuffer<uint>(*geo);
    obj.cbuffer = new ConstantBuffer<Constants>();

    obj.vertices = geo->vertices.data();
    obj.vertexCount = geo->VertexCount();

	scene.push_back(obj);

	changeColor(selectedObject, DEFAULT_COLOR); // cor padrão
	selectedObject = scene.size() - 1; // atualiza o índice do objeto selecionado
    changeColor(selectedObject, SELECTED_COLOR); // cor do novo objeto
}

void BasiCAD::removeObject() {
	if (scene.size() > 0) {
		// remove o objeto selecionado
		scene.erase(scene.begin() + selectedObject);
		
        // ajusta o índice do objeto selecionado
		if (selectedObject >= scene.size())
			selectedObject = scene.size() - 1;

		changeColor(selectedObject, SELECTED_COLOR);
	}
}

void BasiCAD::changeColor(uint index, XMFLOAT4 color) {
	if (index < scene.size()) {
		auto& obj = scene[index];
		
        Vertex* vertices = obj.vertices;

		// percorre os vértices do objeto e altera a cor
		for (uint i = 0; i < obj.vertexCount; ++i) {
			vertices[i].color = color;
		}

		// atualiza o buffer de vértices
		obj.vbuffer->Copy(vertices, obj.vertexCount);
	}

}

// ------------------------------------------------------------------------------
//                                  WinMain                                      
// ------------------------------------------------------------------------------

int APIENTRY WinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPSTR lpCmdLine,
    _In_ int nCmdShow)
{
    try
    {
        // cria motor e configura a janela
        Engine* engine = new Engine();
        engine->window->Mode(ASPECTRATIO);
        engine->window->Size(1024, 720);
        engine->window->Color(25, 25, 25);
        engine->window->Title("BasiCAD");
        engine->window->Icon("Icon");
        engine->window->LostFocus(BasiCAD::Pause);
        engine->window->InFocus(BasiCAD::Resume);

        // cria e executa a aplicação
        engine->Start(new BasiCAD());

        // finaliza execução
        delete engine;
    }
    catch (Error & e)
    {
        // exibe mensagem em caso de erro
        MessageBox(nullptr, e.ToString().data(), "BasiCAD", MB_OK);
    }

    return 0;
}

// ----------------------------------------------------------------------------
