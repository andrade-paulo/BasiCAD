/**********************************************************************************
// Multiple (Arquivo de Cabeçalho)
//
// Criação:     27 Abr 2016
// Atualização: 22 Jun 2025
// Compilador:  Visual C++ 2022
//
// Descrição:   Cena 3D usando múltiplos buffers, um por objeto
//
**********************************************************************************/

#include "DXUT.h"

// ------------------------------------------------------------------------------

struct Constants
{
    XMFLOAT4X4 WorldViewProj =
    { 1.0f, 0.0f, 0.0f, 0.0f,
      0.0f, 1.0f, 0.0f, 0.0f,
      0.0f, 0.0f, 1.0f, 0.0f,
      0.0f, 0.0f, 0.0f, 1.0f };
};

struct Object
{
    XMFLOAT4X4 world = {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f };

    Mesh* mesh = nullptr;
    VertexBuffer<Vertex>* vbuffer = nullptr;
    IndexBuffer<uint>* ibuffer = nullptr;

    ConstantBuffer<Constants>* cbufferPerspective = nullptr;
	ConstantBuffer<Constants>* cbufferFront = nullptr;
	ConstantBuffer<Constants>* cbufferTop = nullptr;
	ConstantBuffer<Constants>* cbufferRight = nullptr;

    Vertex* vertices = nullptr;
	uint vertexCount = 0;
};

// ------------------------------------------------------------------------------

class BasiCAD : public App
{
private:
    ID3D12RootSignature* rootSignature = nullptr;
    ID3D12PipelineState* pipelineState = nullptr;
	ID3D12PipelineState* pipelineStateLine = nullptr;

    // Viewports
    D3D12_VIEWPORT viewportPerspective;
    D3D12_VIEWPORT viewportTop;
    D3D12_VIEWPORT viewportRight;
    D3D12_VIEWPORT viewportFront;
    D3D12_VIEWPORT viewportDivider;
    
	// Vertices para os divisores de viewport
    VertexBuffer<Vertex>* viewportDividerBuffer = nullptr;
	ConstantBuffer<Constants>* viewportDividerCBuffer = nullptr;

    // divisória vertical
	const ushort VERTICAL_DIVIDER = 6;
	Vertex* viewportDividerVertices = new Vertex[VERTICAL_DIVIDER]{
		{ XMFLOAT3(0.0f, -1.0f, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },  // Cima
		{ XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },   // Baixo
		{ XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },   // Centro
		{ XMFLOAT3(-1.0f, 0.0f, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },  // Esquerda
        { XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) },   // Centro
		{ XMFLOAT3(1.0f, 0.0f, 0.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f) }    // Direita
	};
    
	// Projeções
    XMFLOAT4X4 ProjPerspective;
	XMFLOAT4X4 ProjOrtographic;
    
	// Câmera e cena
    OrbitCamera camera;
    static Timer timer;
    vector<Object> scene;

    // Cores
    const XMFLOAT4 DEFAULT_COLOR = Gray;
    const XMFLOAT4 SELECTED_COLOR = Crimson;
    const float MOVEMENT_PACE = 0.1f;

	// Objetos 3D
    Box box = Box(1.0f, 1.0f, 1.0f, SELECTED_COLOR);
    Cylinder cylinder = Cylinder(1.0f, 0.5f, 2.0f, 20, 20, SELECTED_COLOR);
    Sphere sphere = Sphere(1.0f, 20, 20, SELECTED_COLOR);
    Grid grid = Grid(1.0f, 1.0f, 20, 20, SELECTED_COLOR);
	GeoSphere geoSphere = GeoSphere(1.0f, 3, SELECTED_COLOR);

    // Variáveis de controle
	uint selectedObject = 1; // índice do objeto selecionado
	bool multipleView = false; // modo de visualização múltipla

public:
    void Init();
    void Update();
    void Draw();
    void Finalize();

    static void Pause()  { timer.Stop();  }
    static void Resume() { timer.Start(); }

    void BuildRootSignature();
    void BuildPipelineState();

	void processInput();

    void addObj(Geometry* geo);
    void inline addBox() { addObj(&box); }
    void inline addCylinder() { addObj(&cylinder); }
    void inline addSphere() { addObj(&sphere); }
    void inline addGrid() { addObj(&grid); }
	void inline addGeoSphere() { addObj(&geoSphere); }

    void removeObject();

    void changeColor(uint index, XMFLOAT4 color);

    double& gameTime = Engine::frameTime;
};

// ------------------------------------------------------------------------------
