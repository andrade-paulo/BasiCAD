/**********************************************************************************
// IBuffer (Arquivo de Cabe�alho)
//
// Cria��o:     07 Mar 2025
// Atualiza��o: 20 Jun 2025
// Compilador:  Visual C++ 2022
//
// Descri��o:   Representa um recurso do tipo Index Buffer
//
**********************************************************************************/

#ifndef DXUT_IBUFFER_H_
#define DXUT_IBUFFER_H_

// -------------------------------------------------------------------------------

#include "Types.h"
#include "Engine.h"
#include "Graphics.h"
#include <type_traits>
using std::is_same;

// -------------------------------------------------------------------------------

template<typename T>
class IndexBuffer
{
private:
    ID3D12Resource * upload;            // buffer de Upload CPU -> GPU
    ID3D12Resource * ibuffer;           // buffer na GPU
    D3D12_INDEX_BUFFER_VIEW view;       // descritor do buffer 
    uint size;                          // tamanho do buffer 
    DXGI_FORMAT format;                 // formato do buffer 

public:
    // construtor que recebe um buffer
    IndexBuffer<T>(const void * ib, uint indexCount)
    {
        // guarda tamanho do buffer e formato dos �ndices
        size = indexCount * sizeof(T); 
        format = DXGI_FORMAT_R32_UINT;

        // identifica formato dos �ndices de acordo com o tipo T
        if constexpr (is_same<T, ushort>::value)
            format = DXGI_FORMAT_R16_UINT;
        else if constexpr (is_same<T, byte>::value)
            format = DXGI_FORMAT_R8_UINT;

        // aloca recursos para o index buffer
        Engine::graphics->Allocate(UPLOAD, size, &upload);
        Engine::graphics->Allocate(GPU, size, &ibuffer);

        // copia �ndices para o buffer da GPU usando o buffer de Upload
        Engine::graphics->Copy(ib, size, upload, ibuffer);
    }

    // construtor que recebe uma geometria
    IndexBuffer<T>(const Geometry& geo)
        : IndexBuffer<T>(geo.IndexData(), geo.IndexCount())
    { }

    // construtor de c�pia
    IndexBuffer(const IndexBuffer & rhs) = delete;

    // operador de atribui��o
    IndexBuffer & operator=(const IndexBuffer & rhs) = delete;

    // destrutor
    ~IndexBuffer()
    {
        if (upload) upload->Release();
        if (ibuffer) ibuffer->Release();
    }

    D3D12_INDEX_BUFFER_VIEW* View()
    {
        view.BufferLocation = ibuffer->GetGPUVirtualAddress();
        view.Format = format;
        view.SizeInBytes = size;

        return &view;
    } 
};

// -------------------------------------------------------------------------------

#endif



