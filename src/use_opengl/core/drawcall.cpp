

#include "drawcall.hpp"
#include "log/log.hpp"
#include "common/obj_loader.hpp"
#include "common/bmp_loader.hpp"

DrawCall::DrawCall()
{
    Log("DrawCall constructed with default constructor");
}

DrawCall::DrawCall(std::vector<glm::vec3>& vertices, std::vector<glm::vec2>& uvs,
std::vector<glm::vec3>& normals, std::vector<unsigned int>& vboIndices):
    vertices(vertices), uvs(uvs), normals(normals), vboIndices(vboIndices)
{
    Log("DrawCall constructed with copy constructor");
}

DrawCall::DrawCall(std::vector<glm::vec3>&& vertices, std::vector<glm::vec2>&& uvs,
std::vector<glm::vec3>&& normals, std::vector<unsigned int>&& vboIndices):
    vertices(std::move(vertices)), uvs(std::move(uvs)), normals(std::move(normals)), vboIndices(std::move(vboIndices))
{
    Log("DrawCall constructed with move constructor");
}

DrawCall::~DrawCall()
{
    Log("DrawCall destructed");
}

void DrawCall::AddLight(glm::vec3 lightPos, glm::vec3 lightIntensity)
{
    this->lightPos = lightPos;
    this->lightIntensity = lightIntensity;
    this->lightID = glGetUniformLocation(programID, "LightPosition_worldspace");
    this->lightIntID = glGetUniformLocation(programID, "LightIntensity");
}

void DrawCall::AddTexture(std::string texturePath)
{
    this->textureID = loadBMP(texturePath.c_str());
    this->textureUniformID = glGetUniformLocation(programID, "myTextureSampler");
}

void DrawCall::AddModel(std::string modelPath)
{
    loadOBJ(modelPath.c_str(), vertices, uvs, normals, vboIndices);
}

void DrawCall::BufferInit(){
    Log("DrawCall Buffers Initializing...");

    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    //VBO buffer
    glGenBuffers(1, &elementbufferID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbufferID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, vboIndices.size() * sizeof(unsigned int), &vboIndices[0], GL_STATIC_DRAW);
    std::cout<<"Element Buffer Data Size: "<<vboIndices.size()*sizeof(unsigned int)<<std::endl;

    // Buffer vertices and UVs
    glGenBuffers(1, &vertexbufferID);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbufferID);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);
    std::cout<<"Vertex Buffer Data Size: "<<vertices.size() * sizeof(glm::vec3)<<std::endl;
    glGenBuffers(1, &uvbufferID);
    glBindBuffer(GL_ARRAY_BUFFER, uvbufferID);
    glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);
    std::cout<<"UV Buffer Data Size: "<<uvs.size()*sizeof(glm::vec2)<<std::endl;\
    glGenBuffers(1, &normalbufferID);
    glBindBuffer(GL_ARRAY_BUFFER, normalbufferID);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);
    std::cout<<"Normal Buffer Data Size: "<<normals.size()*sizeof(glm::vec3)<<std::endl;

    Log("DrawCall Buffers  Initialized!");
}