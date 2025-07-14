/**********************************************************************************
// OrbitCamera (Arquivo de Cabe�alho)
//
// Cria��o:     21 Jun 2025
// Atualiza��o: 23 Jun 2025
// Compilador:  Visual C++ 2022
//
// Descri��o:   C�mera que orbita em torno da origem
//
**********************************************************************************/

#ifndef DXUT_ORBIT_CAMERA_H
#define DXUT_ORBIT_CAMERA_H

#include <DirectXMath.h>
using namespace DirectX;

// --------------------------------------------------------------------------------

class OrbitCamera
{
private:
    float theta;                // coordenada esf�rica theta 
    float phi;                  // coordenada esf�rica phi
    float radius;               // coordenada esf�rica raio

    float minRadius;            // raio m�nimo
    float maxRadius;            // raio m�ximo
    float moveSpeed;            // velocidade do deslocamento
    float zoomSpeed;            // velocidade do zoom
    
    float lastMouseX;           // �ltima posi��o x do mouse
    float lastMouseY;           // �ltima posi��o y do mouse
    short wheelMouse;           // rolagem do mouse

public:
    float x = 0;                // coordenada cartesiana x
    float y = 0;                // coordenada cartesiana y
    float z = 0;                // coordenada cartesiana z

    // construtor padr�o
    OrbitCamera(float horz = 0, float vert = XM_PIDIV2, float radi = 5.0f);
    
    // atualiza com base no movimento do mouse
    void Update(); 

    void MinRadius(float min);  // define a dist�ncia m�nima da origem
    void MaxRadius(float min);  // define a dist�ncia m�xima da origem
    void MoveSpeed(float vel);  // define a velocidade de movimento
    void ZoomSpeed(float vel);  // define a velocidade do zoom
};

// ------------------------------------------------------------------------------
// M�todos Inline

inline void OrbitCamera::MinRadius(float min)
{ minRadius = min; }

inline void OrbitCamera::MaxRadius(float max)
{ maxRadius = max; }

inline void OrbitCamera::MoveSpeed(float vel)
{ moveSpeed = vel; }

inline void OrbitCamera::ZoomSpeed(float vel)
{ zoomSpeed = vel; }

// ------------------------------------------------------------------------------

#endif

