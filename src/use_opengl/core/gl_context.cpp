#include "gl_context.hpp"
#include "log/log.hpp"
#include "common/gl_check.hpp"

// Clears all draw calls and releases resources
void GLContext::ClearDrawCalls()
{
    // Using smart pointers, so simply clear the vector
    drawCalls.clear();
}

/*
Sets the attribute layout for the draw call.
For MESH and TEXT types of draw calls, different layouts are used.
*/
void SetAttribLayout(const DrawCall* drawCall)
{
    if (drawCall->GetType() == DrawCallType::MESH) {
        // Enable vertex attribute array 0: vertices
        glEnableVertexAttribArray(0);
        CheckGLError("GL Enable Vertex Attrib Array 0");
        glBindBuffer(GL_ARRAY_BUFFER, drawCall->GetVertexBufferID());
        CheckGLError("GL Bind Vertex Buffer");
        glVertexAttribPointer(
            0,        // attribute 0, must match the layout in the shader
            3,        // size
            GL_FLOAT, // type
            GL_FALSE, // normalized?
            0,        // stride
            nullptr   // array buffer offset
        );

        CheckGLError("GL Vertex Buffer");

        // Enable vertex attribute array 1: UVs
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, drawCall->GetUVBufferID());
        glVertexAttribPointer(
            1,        // attribute 1
            2,        // size: U+V --> 2
            GL_FLOAT, // type
            GL_FALSE, // normalized?
            0,        // stride
            nullptr   // array buffer offset
        );

        CheckGLError("GL UV Buffer");

        // Enable vertex attribute array 2: Normals
        glEnableVertexAttribArray(2);
        glBindBuffer(GL_ARRAY_BUFFER, drawCall->GetNormalBufferID());
        glVertexAttribPointer(
            2,        // attribute 2
            3,        // size
            GL_FLOAT, // type
            GL_FALSE, // normalized?
            0,        // stride
            nullptr   // array buffer offset
        );

        CheckGLError("GL Normal Buffer");
    } else if (drawCall->GetType() == DrawCallType::TEXT) {
        // Enable vertex attribute array 0: vertices
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, drawCall->GetVertexBufferID());
        glVertexAttribPointer(
            0,        // attribute 0
            3,        // size
            GL_FLOAT, // type
            GL_FALSE, // normalized?
            0,        // stride
            nullptr   // array buffer offset
        );

        // Enable vertex attribute array 1: UVs
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, drawCall->GetUVBufferID());
        glVertexAttribPointer(
            1,        // attribute 1
            2,        // size: U+V --> 2
            GL_FLOAT, // type
            GL_FALSE, // normalized?
            0,        // stride
            nullptr   // array buffer offset
        );
    }
}

void GLContext::Tick(const glm::vec3& camPosition, const glm::vec3& camDirection)
{
    for (size_t i = 0; i < drawCalls.size(); i++) {
        CheckGLError("GL Context Tick");

        Log("Drawing ", i, "th draw call", " with VAO ", drawCalls[i]->GetVAO());
        glBindVertexArray(drawCalls[i]->GetVAO());

        CheckGLError("GL Bind Vertex Array");
        DrawCall* drawCall = drawCalls[i].get(); // Assuming drawCalls now holds smart pointers

        glm::mat4 ProjectionMatrix = glm::perspective(
            glm::radians(45.0f),
            4.0f / 3.0f,
            0.1f,
            1000.0f
        );

        glm::mat4 ViewMatrix = glm::lookAt(
            camPosition,                  // Camera position
            camPosition + camDirection,   // Look at position
            glm::vec3(0, 1, 0)            // Up vector
        );

        glm::mat4 ModelMatrix = glm::mat4(1.0f);
        glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;

        // Get uniform locations
        MVPMatrixID = glGetUniformLocation(drawCall->GetProgramID(), "MVP");
        ViewMatrixID = glGetUniformLocation(drawCall->GetProgramID(), "V");
        ModelMatrixID = glGetUniformLocation(drawCall->GetProgramID(), "M");

        glUseProgram(drawCall->GetProgramID());

        CheckGLError("GL Use Program");

        // Send transformation matrices to the shader
        glUniformMatrix4fv(MVPMatrixID, 1, GL_FALSE, &MVP[0][0]);
        glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
        glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &ViewMatrix[0][0]);

        CheckGLError("GL Uniform Matrix");

        if (drawCall->GetType() == DrawCallType::MESH) {
            // Bind light properties
            glUniform3fv(drawCall->GetLightID(), 1, &drawCall->GetLightPosition()[0]);
            glUniform1f(drawCall->GetLightIntID(), drawCall->GetLightIntensity());
            CheckGLError("GL Uniforms Light");
        }

        // Bind texture
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, drawCall->GetTextureID());
        glUniform1i(drawCall->GetTextureUniformID(), 0);

        CheckGLError("GL Texture Bind");

        SetAttribLayout(drawCall);

        // Draw call
        if (drawCall->GetType() == DrawCallType::TEXT) {
            glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(drawCall->GetVertices().size()));
        } else if (drawCall->GetType() == DrawCallType::MESH) {
            glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(drawCall->GetVBOIndices().size()), GL_UNSIGNED_INT, nullptr);
        }

        CheckGLError("GL Draw Elements");

        glDisableVertexAttribArray(0);

        CheckGLError("GL Disable Vertex Attrib Array 0");
    }
}

GLContext::GLContext()
{
    Log("GLContext constructed");
}

GLContext::~GLContext()
{
    Log("GLContext destructed");
}
