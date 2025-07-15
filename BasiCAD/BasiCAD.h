/**********************************************************************************
// Multiple (Arquivo de Cabe�alho)
//
// Cria��o:     27 Abr 2016
// Atualiza��o: 22 Jun 2025
// Compilador:  Visual C++ 2022
//
// Descri��o:   Cena 3D usando m�ltiplos buffers, um por objeto
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

    // Viewports
    D3D12_VIEWPORT viewportPerspective;
    D3D12_VIEWPORT viewportTop;
    D3D12_VIEWPORT viewportRight;
    D3D12_VIEWPORT viewportFront;
    
    XMFLOAT4X4 ProjPerspective;
	XMFLOAT4X4 ProjOrtographic;
    
    OrbitCamera camera;
    static Timer timer;
    vector<Object> scene;

    const XMFLOAT4 DEFAULT_COLOR = Gray;
    const XMFLOAT4 SELECTED_COLOR = Crimson;
    const float MOVEMENT_PACE = 0.1;

    Box box = Box(1.0f, 1.0f, 1.0f, SELECTED_COLOR);
    Cylinder cylinder = Cylinder(1.0f, 0.5f, 2.0f, 20, 20, SELECTED_COLOR);
    Sphere sphere = Sphere(1.0f, 20, 20, SELECTED_COLOR);
    Grid grid = Grid(1.0f, 1.0f, 20, 20, SELECTED_COLOR);
	GeoSphere geoSphere = GeoSphere(1.0f, 3, SELECTED_COLOR);

	uint selectedObject = 1; // �ndice do objeto selecionado
	bool multipleView = false; // modo de visualiza��o m�ltipla
public:
    void Init();
    void Update();
    void Draw();
    void Finalize();

    static void Pause()  { timer.Stop();  }
    static void Resume() { timer.Start(); }

    void BuildRootSignature();
    void BuildPipelineState();

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
