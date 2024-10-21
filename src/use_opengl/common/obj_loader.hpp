#pragma once
#include <vector>
#include <GL/glew.h>
#include <unordered_map>
#include <glm/gtc/matrix_transform.hpp>

struct Vertex {
    glm::vec3 position;
    glm::vec2 uv;
    glm::vec3 normal;
    
    Vertex(float x, float y, float z, float u, float v, float nx, float ny, float nz) : position(x, y, z), uv(u, v), normal(nx, ny, nz) {}

    // 实现相等比较
    bool operator==(const Vertex& other) const {
        return position == other.position && uv == other.uv && normal == other.normal;
    }
};

struct VertexHash {
    std::size_t operator()(const Vertex& v) const {
        std::size_t seed = 0;
        // 哈希组合方法：使用一个常量（0x9e3779b9）和位操作来混合各个成员的哈希值
        seed ^= std::hash<float>()(v.position.x) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        seed ^= std::hash<float>()(v.position.y) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        seed ^= std::hash<float>()(v.position.z) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        
        seed ^= std::hash<float>()(v.uv.x) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        seed ^= std::hash<float>()(v.uv.y) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        
        seed ^= std::hash<float>()(v.normal.x) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        seed ^= std::hash<float>()(v.normal.y) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        seed ^= std::hash<float>()(v.normal.z) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        
        return seed;
    }
};

bool loadOBJ(
    const char * path,
    std::vector < glm::vec3 > & out_vertices,
    std::vector < glm::vec2 > & out_uvs,
    std::vector < glm::vec3 > & out_normals,
    std::vector < unsigned int > & vbo_indices_out
);