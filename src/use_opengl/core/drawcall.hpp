#pragma once
#include <vector>
#include "glm/gtc/matrix_transform.hpp"
#include "GL/glew.h"

#include <string>

class DrawCall{
public:
    DrawCall();
    DrawCall(std::vector<glm::vec3>& vertices, std::vector<glm::vec2>& uvs, std::vector<glm::vec3>& normals, std::vector<unsigned int>& vboIndices);
    DrawCall(std::vector<glm::vec3>&& vertices, std::vector<glm::vec2>&& uvs, std::vector<glm::vec3>&& normals, std::vector<unsigned int>&& vboIndices);
    ~DrawCall();

    void BindProgramID(int id){
        this->programID = id;
    }

    void BufferInit();

    void AddLight(glm::vec3 lightPos, glm::vec3 lightIntensity);
    void AddTexture(std::string TexturePath);
    void AddModel(std::string modelPath);

    GLuint GetProgramID(){return this->programID;}
    GLuint GetElementBufferID(){return this->elementbufferID;}
    GLuint GetVertexBufferID(){return this->vertexbufferID;}
    GLuint GetUVBufferID(){return this->uvbufferID;}
    GLuint GetNormalBufferID(){return this->normalbufferID;}
    std::vector<glm::vec3>& GetVertices(){return this->vertices;}
    std::vector<glm::vec2>& GetUVs(){return this->uvs;}
    std::vector<glm::vec3>& GetNormals(){return this->normals;}
    std::vector<unsigned int>& GetVBOIndices(){return this->vboIndices;}
    GLuint GetLightID(){return this->lightID;}
    GLuint GetLightIntID(){return this->lightIntID;}
    GLuint GetTextureID(){return this->textureID;}
    GLuint GetTextureUniformID(){return this->textureUniformID;}
    GLuint GetModelID(){return this->modelID;}


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
    glm::vec3 lightIntensity;
    GLuint lightID;
    GLuint lightIntID;
};