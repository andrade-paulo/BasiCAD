/**********************************************************************************
// ObjRead (Código Fonte)
//
// Descrição: Implementa a leitura de arquivos de modelo 3D no formato .obj
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
        // Se o arquivo não puder ser aberto, exibe uma mensagem de erro.
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

        if (prefix == "v") // Vértice
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

                // Índices em .obj são 1-based, convertendo para 0-based
                face_indices.push_back(v_idx - 1);

                // Ignora o resto do token (vt, vn)
            }

            // Realiza a triangulação por leque (fan triangulation) se a face for um polígono
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

    // Cria os vértices finais com a cor de seleção padrão
    for (const auto& pos : positions)
    {
        loadedGeo->vertices.push_back({ pos, SELECTED_COLOR });
    }

    // Adiciona o objeto carregado à cena e armazena o ponteiro para gerenciamento de memória
    addObj(loadedGeo.get());
    loadedGeometries.push_back(std::move(loadedGeo));
}