#include "text_draw.hpp"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>
#include <vector>
#include <core/core.hpp>
#include <common/gl_shader.hpp>
#include "common/gl_check.hpp"

void printText2D(const char * text, int x, int y, int size, GLContext* context){
    
    DrawCall* drawCall = new DrawCall();
    //drawCall->BindProgramID(LoadShaders("shaders/texts/text_vertex.glsl", "shaders/texts/text_fragment.glsl"));
    CheckGLError("Text Shader Load");

    drawCall->BindProgramID(LoadShaders("shaders/texts/text_vertex.glsl", "shaders/texts/text_fragment.glsl"));
    CheckGLError("Text Shader Load : drawCall->BindProgramID");

    auto& vertices = drawCall->GetVertices();
    auto& uvs = drawCall->GetUVs();
    
    unsigned int length = strlen(text);
    for (unsigned int i = 0; i < length; i++){
        glm::vec3 vertex_up_left = glm::vec3(x+i*size, y+size, 0);
        glm::vec3 vertex_up_right = glm::vec3(x+i*size+size, y+size, 0);
        glm::vec3 vertex_down_right = glm::vec3(x+i*size+size, y, 0);
        glm::vec3 vertex_down_left = glm::vec3(x+i*size, y, 0);

        //convert to Screen Space Coordinates
        auto [resolutionX, resolutionY] = context->GetResolution();
        vertex_down_left.x = (vertex_down_left.x/resolutionX)*2 - 1;
        vertex_down_left.y = (vertex_down_left.y/resolutionY)*2 - 1;
        vertex_down_right.x = (vertex_down_right.x/resolutionX)*2 - 1;
        vertex_down_right.y = (vertex_down_right.y/resolutionY)*2 - 1;
        vertex_up_left.x = (vertex_up_left.x/resolutionX)*2 - 1;
        vertex_up_left.y = (vertex_up_left.y/resolutionY)*2 - 1;
        vertex_up_right.x = (vertex_up_right.x/resolutionX)*2 - 1;
        vertex_up_right.y = (vertex_up_right.y/resolutionY)*2 - 1;


        vertices.push_back(vertex_up_left);
        vertices.push_back(vertex_down_left);
        vertices.push_back(vertex_up_right);

        vertices.push_back(vertex_down_right);
        vertices.push_back(vertex_up_right);
        vertices.push_back(vertex_down_left);

        char character = text[i];
        float uv_x = (character%16)/16.0f;
        float uv_y = (character/16)/16.0f;

        glm::vec2 uv_up_left = glm::vec2(uv_x, 1-uv_y);
        glm::vec2 uv_up_right = glm::vec2(uv_x+1.0f/16.0f, 1-uv_y);
        glm::vec2 uv_down_right = glm::vec2(uv_x+1.0f/16.0f, 1-(uv_y+1.0f/16.0f));
        glm::vec2 uv_down_left = glm::vec2(uv_x, 1-(uv_y+1.0f/16.0f));

        uvs.push_back(uv_up_left);
        uvs.push_back(uv_down_left);
        uvs.push_back(uv_up_right);

        uvs.push_back(uv_down_right);
        uvs.push_back(uv_up_right);
        uvs.push_back(uv_down_left);
    }

    //boot up buffers
    drawCall->SetType(DrawCallType::TEXT);
    drawCall->AddTexture("textures/font.bmp");

    CheckGLError("Text Texture Add");

    drawCall->BufferInit();
    context->AddDrawCall(drawCall);
}