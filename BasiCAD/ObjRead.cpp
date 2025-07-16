/**********************************************************************************
// ObjRead (C�digo Fonte)
//
// Descri��o: Implementa a leitura de arquivos de modelo 3D no formato .obj
//
**********************************************************************************/

#include <vector>
#include "BasiCAD.h"
#include <fstream>
#include <sstream>

void BasiCAD::loadObjFile(const std::string& filename)
{
    std::ifstream file(filename);
    if (!file.is_open())
    {
        // Se o arquivo n�o puder ser aberto, exibe uma mensagem de erro.
        MessageBox(nullptr, ("Falha ao abrir o arquivo: " + filename).c_str(), "Erro de Leitura", MB_OK);
        return;
    }

    auto loadedGeo = std::make_unique<Geometry>();
    std::vector<XMFLOAT3> positions;

    std::string line;
    while (std::getline(file, line))
    {
        std::stringstream ss(line);
        std::string prefix;
        ss >> prefix;

        if (prefix == "v") // V�rtice
        {
            XMFLOAT3 pos;
            ss >> pos.x >> pos.y >> pos.z;
            positions.push_back(pos);
        }
        else if (prefix == "f") // Face
        {
            std::vector<uint> face_indices;
            std::string face_token;
            while (ss >> face_token)
            {
                // Processa cada "v/vt/vn" ou "v"
                std::stringstream token_ss(face_token);
                uint v_idx;
                token_ss >> v_idx;

                // �ndices em .obj s�o 1-based, convertendo para 0-based
                face_indices.push_back(v_idx - 1);

                // Ignora o resto do token (vt, vn)
            }

            // Realiza a triangula��o por leque (fan triangulation) se a face for um pol�gono
            if (face_indices.size() >= 3)
            {
                for (size_t i = 1; i < face_indices.size() - 1; ++i)
                {
                    loadedGeo->indices.push_back(face_indices[0]);
                    loadedGeo->indices.push_back(face_indices[i]);
                    loadedGeo->indices.push_back(face_indices[i + 1]);
                }
            }
        }
    }

    file.close();

    // Cria os v�rtices finais com a cor de sele��o padr�o
    for (const auto& pos : positions)
    {
        loadedGeo->vertices.push_back({ pos, SELECTED_COLOR });
    }

    // Adiciona o objeto carregado � cena e armazena o ponteiro para gerenciamento de mem�ria
    addObj(loadedGeo.get());
    loadedGeometries.push_back(std::move(loadedGeo));
}