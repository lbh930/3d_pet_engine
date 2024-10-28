#pragma once
#include <vector>
#include "glm/gtc/matrix_transform.hpp"
#include "GL/glew.h"

#include <string>

enum class DrawCallType{
    MESH,
    TEXT
};

class DrawCall{
public:
    DrawCall();

    DrawCall(const std::vector<glm::vec3>& vertices, const std::vector<glm::vec2>& uvs,
    const std::vector<glm::vec3>& normals,
    const std::vector<unsigned int>& vboIndices);

    // Move constructor
    DrawCall(DrawCall&& other) noexcept;

    DrawCall(std::vector<glm::vec3>&& vertices, std::vector<glm::vec2>&& uvs, std::vector<glm::vec3>&& normals, std::vector<unsigned int>&& vboIndices);
    ~DrawCall();

    void BindProgramID(int id){
        this->programID = id;
    }

    void BufferInit();

    void AddLight(const glm::vec3& lightPos, const float& lightIntensity);
    void AddTexture(const std::string& texturePath);
    void AddModel(const std::string& modelPath);

    void SetType(DrawCallType type){
        this->type = type;
    }

    DrawCallType GetType() const{
        return this->type;
    }

    GLuint GetProgramID() const{return this->programID;}
    GLuint GetElementBufferID() const{return this->elementbufferID;}
    GLuint GetVertexBufferID() const{return this->vertexbufferID;}
    GLuint GetUVBufferID() const{return this->uvbufferID;}
    GLuint GetNormalBufferID() const{return this->normalbufferID;}
    GLuint GetVAO() const{return this->VAO;}
    std::vector<glm::vec3>& GetVertices() {return this->vertices;}
    std::vector<glm::vec2>& GetUVs() {return this->uvs;}
    std::vector<glm::vec3>& GetNormals() {return this->normals;}
    std::vector<unsigned int>& GetVBOIndices() {return this->vboIndices;}
    GLuint GetLightID() const{return this->lightID;}
    GLuint GetLightIntID() const{return this->lightIntID;}
    glm::vec3 GetLightPosition() const{return this->lightPos;}
    float GetLightIntensity() const{return this->lightIntensity;}
    GLuint GetTextureID() const{return this->textureID;}
    GLuint GetTextureUniformID() const{return this->textureUniformID;}
    GLuint GetModelID() const{return this->modelID;}


private:
    //buffers for rendering
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec2> uvs;
    std::vector<glm::vec3> normals;
    std::vector<unsigned int> vboIndices;

    GLuint programID;
    GLuint elementbufferID;
    GLuint vertexbufferID;
    GLuint uvbufferID;
    GLuint normalbufferID;
    GLuint textureID;
    GLuint textureUniformID;
    GLuint modelID;

    //only support one light for now - will support multiple lights in the future
    glm::vec3 lightPos;
    float lightIntensity;
    GLuint lightID;
    GLuint lightIntID;

    //other
    DrawCallType type;
    GLuint VAO;
};