#include "obj_loader.hpp"
#include "log/log.hpp"
#include <iostream>
#include <vector>
#include <unordered_map>
#include <glm/glm.hpp>
#include <fstream>
#include <sstream>
#include <string>

// Loads an OBJ file and extracts vertex, UV, and normal data.
// Reads vertices, UVs, and normals from the OBJ file and fills output vectors with the parsed data.
// Generate VBO indices by removing duplicate vertices.
// Returns true if the file was loaded successfully, false otherwise.
bool loadOBJ(
    const char* path,
    std::vector<glm::vec3>& out_vertices,
    std::vector<glm::vec2>& out_uvs,
    std::vector<glm::vec3>& out_normals,
    std::vector<unsigned int>& vbo_indices_out
);

bool loadOBJ(
    const char* path,
    std::vector<glm::vec3>& out_vertices,
    std::vector<glm::vec2>& out_uvs,
    std::vector<glm::vec3>& out_normals,
    std::vector<unsigned int>& vbo_indices_out
) {
    std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
    std::vector<glm::vec3> temp_vertices;
    std::vector<glm::vec2> temp_uvs;
    std::vector<glm::vec3> temp_normals;

    std::ifstream file(path);
    if (!file.is_open()) {
        printf("Impossible to open the file!\n");
        return false;
    }

    Log("Loading OBJ file: ", path);

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream lineStream(line);
        std::string lineHeader;
        lineStream >> lineHeader;

        if (lineHeader == "v") {
            glm::vec3 vertex;
            lineStream >> vertex.x >> vertex.y >> vertex.z;
            temp_vertices.push_back(vertex);
        } else if (lineHeader == "vt") {
            glm::vec2 uv;
            lineStream >> uv.x >> uv.y;
            temp_uvs.push_back(uv);
        } else if (lineHeader == "vn") {
            glm::vec3 normal;
            lineStream >> normal.x >> normal.y >> normal.z;
            temp_normals.push_back(normal);
        } else if (lineHeader == "f") {
            unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
            char slash;  // to consume '/'
            for (int i = 0; i < 3; i++) {
                lineStream >> vertexIndex[i] >> slash >> uvIndex[i] >> slash >> normalIndex[i];
                vertexIndices.push_back(vertexIndex[i]);
                uvIndices.push_back(uvIndex[i]);
                normalIndices.push_back(normalIndex[i]);
            }
        }
    }

    Log("OBJ file loaded: ", path, " with ", temp_vertices.size(), " vertices");

    std::unordered_map<Vertex, unsigned int, VertexHash> vbo_map;

    // For each vertex of each triangle
    for (size_t i = 0; i < vertexIndices.size(); i++) {
        unsigned int vertexIndex = vertexIndices[i];
        glm::vec3 vertex = temp_vertices[vertexIndex - 1];

        unsigned int uvIndex = uvIndices[i];
        glm::vec2 uv = temp_uvs[uvIndex - 1];

        unsigned int normalIndex = normalIndices[i];
        glm::vec3 normal = temp_normals[normalIndex - 1];

        Vertex vbo_vertex = {vertex.x, vertex.y, vertex.z, uv.x, uv.y, normal.x, normal.y, normal.z};
        if (vbo_map.find(vbo_vertex) == vbo_map.end()) {
            unsigned int newIndex = static_cast<unsigned int>(vbo_map.size());
            vbo_map[vbo_vertex] = newIndex;
            out_vertices.push_back(vertex);
            out_uvs.push_back(uv);
            out_normals.push_back(normal);
            vbo_indices_out.push_back(newIndex);
        } else {
            vbo_indices_out.push_back(vbo_map[vbo_vertex]);
        }
    }

    Log("OBJ load: VBO Indices: ", vbo_indices_out.size());

    return true;
}
