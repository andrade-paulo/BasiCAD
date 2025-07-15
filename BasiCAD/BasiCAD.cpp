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
    XMStoreFloat4x4(&ProjPerspective, 
        XMMatrixPerspectiveFovLH(
        XMConvertToRadians(45.0f), 
        window->AspectRatio(), 
        1.0f, 100.0f)
    );

	float ortographicHeight = 5.0f;
	float ortographicWidth = ortographicHeight * window->AspectRatio();

	XMStoreFloat4x4(&ProjOrtographic, 
        XMMatrixOrthographicLH(
		ortographicWidth, ortographicHeight, 1.0f, 100.0f)
    );

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
    viewportFront.TopLeftX = 0.0f;
    viewportFront.TopLeftY = 0.0f;
    viewportFront.Width = float(window->Width() / 2.0f);
    viewportFront.Height = float(window->Height() / 2.0f);
    viewportFront.MinDepth = 0.0f;
    viewportFront.MaxDepth = 1.0f;

    // lado direito
    viewportRight.TopLeftX = float(window->Width() / 2.0f);
    viewportRight.TopLeftY = 0.0f;
    viewportRight.Width = float(window->Width() / 2.0f);
    viewportRight.Height = float(window->Height() / 2.0f);
    viewportRight.MinDepth = 0.0f;
    viewportRight.MaxDepth = 1.0f;

	// topo
	viewportTop.TopLeftX = 0.0f;
	viewportTop.TopLeftY = float(window->Height() / 2.0f);
	viewportTop.Width = float(window->Width() / 2.0f);
	viewportTop.Height = float(window->Height() / 2.0f);
	viewportTop.MinDepth = 0.0f;
	viewportTop.MaxDepth = 1.0f;

	// perspectiva
	viewportPerspective.TopLeftX = float(window->Width() / 2.0f);
	viewportPerspective.TopLeftY = float(window->Height() / 2.0f);
	viewportPerspective.Width = float(window->Width() / 2.0f);
	viewportPerspective.Height = float(window->Height() / 2.0f);
	viewportPerspective.MinDepth = 0.0f;
	viewportPerspective.MaxDepth = 1.0f;

	// viewport de divisores
	viewportDivider.TopLeftX = 0.0f;
	viewportDivider.TopLeftY = 0.0f;
	viewportDivider.Width = float(window->Width());
	viewportDivider.Height = float(window->Height());
	viewportDivider.MinDepth = 0.0f;
	viewportDivider.MaxDepth = 1.0f;

	// cria o vbuffer e cbuffer para os divisores de viewport
	viewportDividerBuffer = new VertexBuffer<Vertex>(viewportDividerVertices, VERTICAL_DIVIDER);
    viewportDividerCBuffer = new ConstantBuffer<Constants>();
 
    // ---------------------

    BuildRootSignature();
    BuildPipelineState();    

    // ----------------------

    timer.Start();
}

// ------------------------------------------------------------------------------

void BasiCAD::Update()
{
    processInput();

    // atualiza posição da câmera
    camera.Update();

    // constrói a matriz de visualização
    XMVECTOR target = XMVectorZero();

    XMVECTOR posPerspective = XMVectorSet(camera.x, camera.y, camera.z, 1.0f);
    XMVECTOR upPerspective = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
    XMMATRIX viewPerspective = XMMatrixLookAtLH(posPerspective, target, upPerspective);

    // carrega matriz de projeção
    XMMATRIX projPerspective = XMLoadFloat4x4(&ProjPerspective);

    if (multipleView) {
		XMMATRIX projOrtographic = XMLoadFloat4x4(&ProjOrtographic);
        
		// Visualização para as outras vistas
		XMVECTOR posFront = XMVectorSet(0.0f, 0.0f, -5.0f, 1.0f);
		XMVECTOR upFront = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
		XMMATRIX viewFrontMatrix = XMMatrixLookAtLH(posFront, target, upFront);

		XMVECTOR posTop = XMVectorSet(0.0f, 5.0f, 0.0f, 1.0f);
		XMVECTOR upTop = XMVectorSet(0.0f, 0.0f, -1.0f, 0.0f);
		XMMATRIX viewTopMatrix = XMMatrixLookAtLH(posTop, target, upTop);
        
		XMVECTOR posRight = XMVectorSet(5.0f, 0.0f, 0.0f, 1.0f);
		XMVECTOR upRight = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
		XMMATRIX viewRightMatrix = XMMatrixLookAtLH(posRight, target, upRight);

        Constants constants;

        for (auto& obj : scene)
        {
            // carrega matriz de mundo
            XMMATRIX world = XMLoadFloat4x4(&obj.world);

            // constrói matriz combinada
            XMMATRIX WorldViewProjPerspective = world * viewPerspective * projPerspective;
			XMMATRIX WorldViewProjFront = world * viewFrontMatrix * projOrtographic;
			XMMATRIX WorldViewProjTop = world * viewTopMatrix * projOrtographic;
			XMMATRIX WorldViewProjRight = world * viewRightMatrix * projOrtographic;

			// insere no buffer cada matriz combinada
            XMStoreFloat4x4(&constants.WorldViewProj, XMMatrixTranspose(WorldViewProjPerspective));
            obj.cbufferPerspective->Copy(&constants);

            XMStoreFloat4x4(&constants.WorldViewProj, XMMatrixTranspose(WorldViewProjFront));
			obj.cbufferFront->Copy(&constants);
            
			XMStoreFloat4x4(&constants.WorldViewProj, XMMatrixTranspose(WorldViewProjTop));
			obj.cbufferTop->Copy(&constants);
            
			XMStoreFloat4x4(&constants.WorldViewProj, XMMatrixTranspose(WorldViewProjRight));
			obj.cbufferRight->Copy(&constants);
        }

		// atualiza o buffer de divisórias com a identidade
        XMStoreFloat4x4(&constants.WorldViewProj, XMMatrixIdentity());
        viewportDividerCBuffer->Copy(&constants);
    }
    else {
        // ajusta o constant buffer de cada objeto
        for (auto& obj : scene)
        {
            // carrega matriz de mundo
            XMMATRIX world = XMLoadFloat4x4(&obj.world);

            // constrói matriz combinada
            XMMATRIX WorldViewProj = world * viewPerspective * projPerspective;

            // atualiza o buffer constante com a matriz combinada
            Constants constants;
            XMStoreFloat4x4(&constants.WorldViewProj, XMMatrixTranspose(WorldViewProj));
            obj.cbufferPerspective->Copy(&constants);
        }
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

    if (multipleView) {
		D3D12_VIEWPORT viewports[] = { viewportPerspective, viewportFront, viewportTop, viewportRight };

		// desenha objetos da cena para cada viewport
        for (auto& obj : scene)
        {
            D3D12_GPU_VIRTUAL_ADDRESS views[] = { obj.cbufferPerspective->View(), obj.cbufferFront->View(), obj.cbufferTop->View(), obj.cbufferRight->View() };

			for (size_t i = 0; i < 4; ++i) {
				// define a viewport atual
				graphics->CommandList()->RSSetViewports(1, &viewports[i]);
				graphics->CommandList()->SetGraphicsRootConstantBufferView(0, views[i]);

                graphics->CommandList()->IASetVertexBuffers(0, 1, obj.vbuffer->View());
                graphics->CommandList()->IASetIndexBuffer(obj.ibuffer->View());
                graphics->CommandList()->DrawIndexedInstanced(obj.mesh->indexCount, 1, obj.mesh->startIndex, obj.mesh->baseVertex, 0);
			}
        }

		// configura pipeline e viewport para as linhas de divisão
		graphics->CommandList()->RSSetViewports(1, &viewportDivider);
		graphics->CommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINELIST);

        graphics->CommandList()->SetPipelineState(pipelineStateLine);

        // desenho das divisórias
		graphics->CommandList()->SetGraphicsRootConstantBufferView(0, viewportDividerCBuffer->View());
		graphics->CommandList()->IASetVertexBuffers(0, 1, viewportDividerBuffer->View());
		graphics->CommandList()->DrawInstanced(VERTICAL_DIVIDER, 1, 0, 0);

    }
    else {
        // desenha objetos da cena
        for (auto& obj : scene)
        {
            // comandos de configuração específicos a cada objeto
            graphics->CommandList()->SetGraphicsRootConstantBufferView(0, obj.cbufferPerspective->View());
            graphics->CommandList()->IASetVertexBuffers(0, 1, obj.vbuffer->View());
            graphics->CommandList()->IASetIndexBuffer(obj.ibuffer->View());

            // desenha objeto
            graphics->CommandList()->DrawIndexedInstanced(
                obj.mesh->indexCount, 1,
                obj.mesh->startIndex,
                obj.mesh->baseVertex,
                0
            );
        }
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
	pipelineStateLine->Release();

    for (auto& obj : scene)
    {
        delete obj.mesh;
        delete obj.vbuffer;
        delete obj.ibuffer;
        delete obj.cbufferPerspective;
		delete obj.cbufferFront;
		delete obj.cbufferTop;
		delete obj.cbufferRight;
    }
}

// ------------------------------------------------------------------------------
void BasiCAD::addObj(Geometry* geo) {
	Object obj;

	XMStoreFloat4x4(&obj.world, XMMatrixIdentity());

	obj.mesh = new Mesh(*geo);
    obj.vbuffer = new VertexBuffer<Vertex>(*geo);
    obj.ibuffer = new IndexBuffer<uint>(*geo);

    obj.cbufferPerspective = new ConstantBuffer<Constants>();
	obj.cbufferFront = new ConstantBuffer<Constants>();
	obj.cbufferTop = new ConstantBuffer<Constants>();
	obj.cbufferRight = new ConstantBuffer<Constants>();

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
