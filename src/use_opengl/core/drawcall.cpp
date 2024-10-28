#include "drawcall.hpp"
#include "log/log.hpp"
#include "common/obj_loader.hpp"
#include "common/bmp_loader.hpp"
#include "common/gl_check.hpp"

// DrawCall default constructor
DrawCall::DrawCall()
{
    Log("DrawCall constructed with default constructor");
    // No need to explicitly initialize vectors; they are default-initialized
}

// DrawCall copy constructor
DrawCall::DrawCall(const std::vector<glm::vec3>& vertices, const std::vector<glm::vec2>& uvs,
    const std::vector<glm::vec3>& normals,
    const std::vector<unsigned int>& vboIndices)
    : vertices(vertices),
    uvs(uvs),
    normals(normals),
    vboIndices(vboIndices)
{
    Log("DrawCall constructed with copy constructor");
}

// DrawCall move constructor
DrawCall::DrawCall(std::vector<glm::vec3>&& vertices, std::vector<glm::vec2>&& uvs,
    std::vector<glm::vec3>&& normals, std::vector<unsigned int>&& vboIndices)
    : vertices(std::move(vertices)),
    uvs(std::move(uvs)),
    normals(std::move(normals)),
    vboIndices(std::move(vboIndices))
{
    Log("DrawCall constructed with move constructor");
}
#include "drawcall.hpp"
#include <utility> // for std::move

// Move constructor
DrawCall::DrawCall(DrawCall&& other) noexcept
    : vertices(std::move(other.vertices)),
      uvs(std::move(other.uvs)),
      normals(std::move(other.normals)),
      vboIndices(std::move(other.vboIndices)),
      programID(other.programID),
      elementbufferID(other.elementbufferID),
      vertexbufferID(other.vertexbufferID),
      uvbufferID(other.uvbufferID),
      normalbufferID(other.normalbufferID),
      textureID(other.textureID),
      VAO(other.VAO) 
{
    // Reset other's resources to prevent double deletion
    other.programID = 0;
    other.elementbufferID = 0;
    other.vertexbufferID = 0;
    other.uvbufferID = 0;
    other.normalbufferID = 0;
    other.textureID = 0;
    other.VAO = 0;
}

// DrawCall destructor
DrawCall::~DrawCall()
{
    Log("DrawCall destructed");
    // Delete buffers and resources if they exist

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

    // Clear the vectors
    vertices.clear();
    uvs.clear();
    normals.clear();
    vboIndices.clear();

    CheckGLError("DrawCall Destructor");
}

void DrawCall::AddLight(const glm::vec3& lightPos, const float & lightIntensity)
{
    this->lightPos = lightPos;
    this->lightIntensity = lightIntensity;
    this->lightID = glGetUniformLocation(programID, "LightPosition_worldspace");
    this->lightIntID = glGetUniformLocation(programID, "LightIntensity");
}

void DrawCall::AddTexture(std::vector<unsigned char>& textureData, const std::pair<int, int>& textureSize)
{
    // Create one OpenGL texture
    GLuint textureID;
    glGenTextures(1, &textureID);

    // "Bind" the newly created texture: all future texture functions will modify this texture
    glBindTexture(GL_TEXTURE_2D, textureID);

    // Give the image to OpenGL
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureSize.first, textureSize.second, 0, GL_BGR, GL_UNSIGNED_BYTE, textureData.data());

    // Set the texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);  // When magnifying, use linear filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);  // When minifying, use linear mipmap linear filtering

    // Generate mipmaps
    glGenerateMipmap(GL_TEXTURE_2D);
    this->textureUniformID = glGetUniformLocation(programID, "myTextureSampler");
}

void DrawCall::AddModel(const std::string& modelPath)
{
    bool success = loadOBJ(modelPath.c_str(), vertices, uvs, normals, vboIndices);
    if (!success) {
        Error("Failed to load OBJ file: ", modelPath);
    } else {
        Log("OBJ file loaded successfully: ", modelPath);
    }
}

void DrawCall::BufferInit()
{
    Log("DrawCall Buffers Initializing...");

    // Bind Vertex Array Object (VAO)
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    Log("VAO: ", VAO);

    CheckGLError("GL Gen and Bind Vertex Array");

    if (this->type == DrawCallType::TEXT) {
        Log("DrawCall Type: TEXT");
    } else {
        Log("DrawCall Type: MESH");

        if (!vboIndices.empty()) {
            glGenBuffers(1, &elementbufferID);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbufferID);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, vboIndices.size() * sizeof(unsigned int), vboIndices.data(), GL_STATIC_DRAW);
            Log("Element Buffer Data Size: ", vboIndices.size() * sizeof(unsigned int));
        } else {
            Error("vboIndices is empty in BufferInit()");
            return;
        }

        if (!normals.empty()) {
            glGenBuffers(1, &normalbufferID);
            glBindBuffer(GL_ARRAY_BUFFER, normalbufferID);
            glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), normals.data(), GL_STATIC_DRAW);
            Log("Normal Buffer Data Size: ", normals.size() * sizeof(glm::vec3));
        } else {
            Error("Normals vector is empty in BufferInit()");
        }
    }

    Log("Vertices Size: ", vertices.size());
    if (!vertices.empty()) {
        glGenBuffers(1, &vertexbufferID);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbufferID);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);
        Log("Vertex Buffer Data Size: ", vertices.size() * sizeof(glm::vec3));
    } else {
        Error("Vertices vector is empty in BufferInit()");
        return;
    }

    Log("UVs Size: ", uvs.size());
    if (!uvs.empty()) {
        glGenBuffers(1, &uvbufferID);
        glBindBuffer(GL_ARRAY_BUFFER, uvbufferID);
        glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), uvs.data(), GL_STATIC_DRAW);
        Log("UV Buffer Data Size: ", uvs.size() * sizeof(glm::vec2));
    } else {
        Error("UVs vector is empty in BufferInit()");
        return;
    }

    Log("DrawCall Buffers Initialized!");
}
