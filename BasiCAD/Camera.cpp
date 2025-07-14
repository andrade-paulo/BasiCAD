/**********************************************************************************
// OrbitCamera (C�digo Fonte)
//
// Cria��o:     21 Jun 2025
// Atualiza��o: 23 Jun 2025
// Compilador:  Visual C++ 2022
//
// Descri��o:   C�mera que orbita em torno da origem
//
**********************************************************************************/

#include "Camera.h"
#include "Engine.h"

// ------------------------------------------------------------------------------

OrbitCamera::OrbitCamera(float horz, float vert, float radi)
    : theta(horz)
    , phi(vert)
    , radius(radi)
{
    // mant�m phi na faixa ]0-180[
    phi = max(0.001f, min(XM_PI - 0.001f, phi));

    // raio m�nimo e m�ximo
    minRadius = radius / 2.0f;
    maxRadius = 3.0f * radius;

    // velocidade de movimenta��o
    moveSpeed = 0.250f;
    zoomSpeed = 0.005f;

    // �ltima posi��o do mouse
    lastMouseX = (float) Engine::input->MouseX();
    lastMouseY = (float) Engine::input->MouseY();
}

// ------------------------------------------------------------------------------

void OrbitCamera::Update()
{
    // posi��o atual do mouse
    float mouseX = (float) Engine::input->MouseX();
    float mouseY = (float) Engine::input->MouseY();

    // ---------------------
    // Movimento da C�mera
    // ---------------------

    if (Engine::input->KeyDown(VK_LBUTTON))
    {
        // cada pixel corresponde a 'moveSpeed' graus
        float dx = XMConvertToRadians(moveSpeed * (mouseX - lastMouseX));
        float dy = XMConvertToRadians(moveSpeed * (mouseY - lastMouseY));

        // atualiza �ngulos com base no deslocamento do mouse
        theta += dx;
        phi += dy;

        // restringe o �ngulo de phi ]0-180[ graus
        phi = max(0.001f, min(XM_PI - 0.001f, phi));
    }

    // ---------------------
    // Zoom da C�mera
    // ---------------------
    else if (Engine::input->KeyDown(VK_RBUTTON))
    {
        // cada pixel corresponde a 'zoomSpeed' unidades
        float dy = zoomSpeed * (mouseY - lastMouseY);

        // atualiza o raio da c�mera com base no deslocamento do mouse 
        radius += dy;

        // restringe o raio entre minRadius e maxRadius
        radius = max(minRadius, min(radius, maxRadius));
    }
    else if (wheelMouse = Engine::input->MouseWheel())
    {
        // atualiza o raio da c�mera com base na rolagem do mouse 
        radius += zoomSpeed * (wheelMouse);

        // restringe o raio entre minRadius e maxRadius
        radius = max(minRadius, min(radius, maxRadius));
    }

    // ------------------------
    // Coordenadas cartesianas
    // ------------------------

    // converte coordenadas esf�ricas para cartesianas
    x = radius * sinf(phi) * cosf(theta);
    z = radius * sinf(phi) * sinf(theta);
    y = radius * cosf(phi);

    // atualiza �ltima posi��o do mouse
    lastMouseX = mouseX;
    lastMouseY = mouseY;
}

// ------------------------------------------------------------------------------