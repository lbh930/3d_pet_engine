#include "text_draw.hpp"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>
#include <vector>
#include <core/core.hpp>
#include <common/gl_shader.hpp>
#include "common/gl_check.hpp"
#include "log/log.hpp"
#include <memory>
#include <string_view>
#include <common/bmp_loader.hpp>


// Prints text in 2D using the provided GLContext
void printText2D(std::string_view text, int x, int y, int size, GLContext* context) {
    Log("Printing Text");

    // Use smart pointer to manage DrawCall memory
    auto drawCall = std::make_unique<DrawCall>();

    CheckGLError("Text Shader Load");

    // Bind the shader program
    GLuint programID = LoadShaders("shaders/texts/text_vertex.glsl", "shaders/texts/text_fragment.glsl");
    drawCall->BindProgramID(programID);
    CheckGLError("Text Shader Load : drawCall->BindProgramID");
    Log("Text Shader Loaded", programID);

    auto& vertices = drawCall->GetVertices();
    auto& uvs = drawCall->GetUVs();

    unsigned int length = static_cast<unsigned int>(text.size());

    Log("Text Length: ", length);

    // Get screen resolution
    auto [resolutionX, resolutionY] = context->GetResolution();

    // Constants for UV mapping
    constexpr float fontGridSize = 16.0f;
    constexpr float uvUnit = 1.0f / fontGridSize;

    // Lambda function to convert to NDC coordinates
    auto convertToNDC = [resolutionX, resolutionY](glm::vec3& vertex) {
        vertex.x = (vertex.x / resolutionX) * 2.0f - 1.0f;
        vertex.y = (vertex.y / resolutionY) * 2.0f - 1.0f;
    };

    for (unsigned int i = 0; i < length; i++) {
        float x_pos = x + i * size;
        float y_pos = y;

        glm::vec3 vertex_up_left     = glm::vec3(x_pos, y_pos + size, 0);
        glm::vec3 vertex_up_right    = glm::vec3(x_pos + size, y_pos + size, 0);
        glm::vec3 vertex_down_right  = glm::vec3(x_pos + size, y_pos, 0);
        glm::vec3 vertex_down_left   = glm::vec3(x_pos, y_pos, 0);

        // Convert to Screen Space Coordinates
        convertToNDC(vertex_up_left);
        convertToNDC(vertex_up_right);
        convertToNDC(vertex_down_right);
        convertToNDC(vertex_down_left);

        // Add vertices to the list
        vertices.push_back(vertex_up_left);
        vertices.push_back(vertex_down_left);
        vertices.push_back(vertex_up_right);

        vertices.push_back(vertex_down_right);
        vertices.push_back(vertex_up_right);
        vertices.push_back(vertex_down_left);

        // Calculate UV coordinates
        char character = text[i];
        float uv_x = (character % static_cast<int>(fontGridSize)) / fontGridSize;
        float uv_y = (character / static_cast<int>(fontGridSize)) / fontGridSize;

        glm::vec2 uv_up_left     = glm::vec2(uv_x, 1.0f - uv_y);
        glm::vec2 uv_up_right    = glm::vec2(uv_x + uvUnit, 1.0f - uv_y);
        glm::vec2 uv_down_right  = glm::vec2(uv_x + uvUnit, 1.0f - (uv_y + uvUnit));
        glm::vec2 uv_down_left   = glm::vec2(uv_x, 1.0f - (uv_y + uvUnit));

        // Add UVs to the list
        uvs.push_back(uv_up_left);
        uvs.push_back(uv_down_left);
        uvs.push_back(uv_up_right);

        uvs.push_back(uv_down_right);
        uvs.push_back(uv_up_right);
        uvs.push_back(uv_down_left);
    }

    Log("Text Vertices: ", vertices.size());

    // Initialize buffers and add the draw call to the context
    drawCall->SetType(DrawCallType::TEXT);

    std::vector<unsigned char> textureData;
    std::pair <int, int> textureSize;
    loadBMP("textures/font.bmp", textureData, textureSize);
    drawCall->AddTexture(textureData, textureSize);

    CheckGLError("Text Texture Add");

    drawCall->BufferInit();
    context->AddDrawCall(std::move(drawCall));

    Log("Text Drawn");
}
