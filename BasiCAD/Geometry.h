/**********************************************************************************
// Geometry (Arquivo de Cabe�alho)
//
// Cria��o:     03 Fev 2013
// Atualiza��o: 05 Mar 2025
// Compilador:  Visual C++ 2022
//
// Descri��o:   Define v�rtices e �ndices para v�rias geometrias
//
**********************************************************************************/

#ifndef DXUT_GEOMETRY_H_
#define DXUT_GEOMETRY_H_

// -------------------------------------------------------------------------------

#include "Types.h"
#include <vector>
#include <DirectXMath.h>
#include <DirectXColors.h>
#include "Colors.h"
using namespace DirectX;
using std::vector;

// -------------------------------------------------------------------------------

struct Vertex
{
    XMFLOAT3 pos;
    XMFLOAT4 color;
};

// -------------------------------------------------------------------------------
// Geometry
// -------------------------------------------------------------------------------

class Geometry
{
protected:
    void Subdivide();                       // subdivide tri�ngulos

public:
    vector<Vertex> vertices;                // v�rtices da geometria
    vector<uint> indices;                   // �ndices da geometria

    // m�todos inline
    const Vertex* VertexData() const        // retorna v�rtices da geometria
    { return vertices.data(); }

    const uint* IndexData() const           // retorna �ndices da geometria
    { return indices.data(); }

    uint VertexCount() const                // retorna n�mero de v�rtices
    { return uint(vertices.size()); }

    uint IndexCount() const                 // retorna n�mero de �ndices
    { return uint(indices.size()); }
};

// -------------------------------------------------------------------------------
// Box
// -------------------------------------------------------------------------------

struct Box : public Geometry
{
    Box(float width,                        // largura
        float height,                       // altura
        float depth,                        // profundidade
        XMFLOAT4 color = Wood);             // cor
};

// -------------------------------------------------------------------------------
// Cylinder
// -------------------------------------------------------------------------------

struct Cylinder : public Geometry
{
    Cylinder(float bottom,                  // raio inferior
        float top,                          // raio superior
        float height,                       // altura
        uint sliceCount,                    // n�mero de fatias
        uint stackCount,                    // n�mero de camadas
        XMFLOAT4 color = Yellow);           // cor
};

// -------------------------------------------------------------------------------
// Sphere
// -------------------------------------------------------------------------------

struct Sphere : public Geometry
{
    Sphere(float radius,                    // raio da esfera
        uint sliceCount,                    // n�mero de fatias
        uint stackCount,                    // n�mero de camadas
        XMFLOAT4 color = Crimson);          // cor
};

// -------------------------------------------------------------------------------
// GeoSphere
// -------------------------------------------------------------------------------

struct GeoSphere : public Geometry
{
    GeoSphere(float radius,                 // raio da geoesfera
        uint subdivisions,                  // n�mero de subdivis�es (m�ximo 6)
        XMFLOAT4 color = Orange);           // cor
};


// -------------------------------------------------------------------------------
// Grid
// -------------------------------------------------------------------------------

struct Grid : public Geometry
{
    Grid(float width,                       // largura
        float depth,                        // profundidade
        uint m,                             // n�mero de linhas
        uint n,                             // n�mero de colunas
        XMFLOAT4 color = Gray);             // cor
};

// -------------------------------------------------------------------------------
// Quad
// -------------------------------------------------------------------------------

struct Quad : public Geometry
{
    Quad(float width,                       // largura
        float height,                       // altura
        XMFLOAT4 color = Green);            // cor
};

// -------------------------------------------------------------------------------

#endif
