

#include "drawcall.hpp"
#include "log/log.hpp"
#include "common/obj_loader.hpp"
#include "common/bmp_loader.hpp"
#include "common/gl_check.hpp"

DrawCall::DrawCall()
{
    Log("DrawCall constructed with default constructor");
    //make sure every vector exists
    vertices = std::vector<glm::vec3>();
    uvs = std::vector<glm::vec2>();
    normals = std::vector<glm::vec3>();
    vboIndices = std::vector<unsigned int>();
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
    //delete buffers

    if (programID != 0) {
        glDeleteProgram(programID);
    }

    if (elementbufferID != 0) {
        glDeleteBuffers(1, &elementbufferID);
    }

    if (vertexbufferID != 0) {
        glDeleteBuffers(1, &vertexbufferID);
    }

    if (uvbufferID != 0) {
        glDeleteBuffers(1, &uvbufferID);
    }

    if (normalbufferID != 0) {
        glDeleteBuffers(1, &normalbufferID);
    }

    if (textureID != 0) {
        glDeleteTextures(1, &textureID);
    }

    if (VAO != 0) {
        glDeleteVertexArrays(1, &VAO);
    }

    vertices.clear();
    uvs.clear();
    normals.clear();
    vboIndices.clear();

    CheckGLError("DrawCall Destructor");

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
    CheckGLError("Before Load BMP");
    this->textureID = loadBMP(texturePath.c_str());
    CheckGLError("After Load BMP");
    this->textureUniformID = glGetUniformLocation(programID, "myTextureSampler");
    CheckGLError("After Get Uniform Texture");
}

void DrawCall::AddModel(std::string modelPath)
{
    bool success = loadOBJ(modelPath.c_str(), vertices, uvs, normals, vboIndices);
    if (!success) {
        Error("Failed to load OBJ file: ", modelPath);
    }else{
        Log("OBJ file loaded successfully: ", modelPath);
    }

}
void DrawCall::BufferInit(){
    Log("DrawCall Buffers Initializing...");

    //bind Vertex Array Object (VAO)
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    Log("VAO: ", VAO);

    CheckGLError("GL Gen and Bind Vertex Array");

    if (this->type == DrawCallType::TEXT){
        Log("DrawCall Type: TEXT");
    } else {
        Log("DrawCall Type: MESH");

        if (!vboIndices.empty()) {
            glGenBuffers(1, &elementbufferID);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbufferID);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, vboIndices.size() * sizeof(unsigned int), &vboIndices[0], GL_STATIC_DRAW);
            Log("Element Buffer Data Size: ", vboIndices.size() * sizeof(unsigned int));
        } else {
            Error("vboIndices is empty in BufferInit()");
            return;
        }

        if (!normals.empty()) {
            glGenBuffers(1, &normalbufferID);
            glBindBuffer(GL_ARRAY_BUFFER, normalbufferID);
            glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);
            Log("Normal Buffer Data Size: ", normals.size() * sizeof(glm::vec3));
        } else {
            Error("Normals vector is empty in BufferInit()");
        }
    }

    Log("Vertices Size: ", vertices.size());
    if (!vertices.empty()) {
        glGenBuffers(1, &vertexbufferID);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbufferID);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);
        Log("Vertex Buffer Data Size: ", vertices.size() * sizeof(glm::vec3));
    } else {
        Error("Vertices vector is empty in BufferInit()");
        return;
    }

    Log("UVs Size: ", uvs.size());
    if (!uvs.empty()) {
        glGenBuffers(1, &uvbufferID);
        glBindBuffer(GL_ARRAY_BUFFER, uvbufferID);
        glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);
        Log("UV Buffer Data Size: ", uvs.size() * sizeof(glm::vec2));
    } else {
        Error("UVs vector is empty in BufferInit()");
        return;
    }

    Log("DrawCall Buffers Initialized!");
}
