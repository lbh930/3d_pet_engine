
#include "gl_context.hpp"
#include "log/log.hpp"
#include "common/gl_check.hpp"

void GLContext::ClearDrawCalls(){
    //I love manual memory management
    
    drawCalls.clear();
}

/*
Set the attribute layout for the draw call
For MESH and TEXT type of draw calls, different layout are used.
*/
void SetAttribLayout(DrawCall* drawCall){
    if (drawCall->GetType() == DrawCallType::MESH){
        // Draw
        // 0 - attribute buffer : vertices
        glEnableVertexAttribArray(0);
        CheckGLError("GL Enable Vertex Attrib Array 0");
        glBindBuffer(GL_ARRAY_BUFFER, drawCall->GetVertexBufferID());
        CheckGLError("GL Bind Vertex Buffer");
        glVertexAttribPointer(
        0,// attribute 0. No particular reason for 0, but must match the layout in the shader.
        3, // size
        GL_FLOAT,// type
        GL_FALSE, // normalized?
        0, // stride
        (void*)0 // array buffer offset
        );

        CheckGLError("GL Vertex Buffer");

        // 1 - attribute buffer : UVs
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, drawCall->GetUVBufferID());
        glVertexAttribPointer(
            1,                                // attribute. No particular reason for 2, but must match the layout in the shader.
            2,                                // size : U+V --> 2
            GL_FLOAT,                         // type
            GL_FALSE,                         // normalized?
            0,                                // stride
            (void*)0                          // array buffer offset
        );

        CheckGLError("GL UV Buffer");

        // 2 - Normals
        glEnableVertexAttribArray(2);
        glBindBuffer(GL_ARRAY_BUFFER, drawCall->GetNormalBufferID());
        glVertexAttribPointer(
            2,                                // attribute
            3,                                // size
            GL_FLOAT,                         // type
            GL_FALSE,                         // normalized?
            0,                                // stride
            (void*)0                          // array buffer offset
        );

        CheckGLError("GL Normal Buffer");
    }else if(drawCall->GetType() == DrawCallType::TEXT){
        // Draw
        // 0 - attribute buffer : vertices
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, drawCall->GetVertexBufferID());
        glVertexAttribPointer(
        0,// attribute 0. No particular reason for 0, but must match the layout in the shader.
        3, // size
        GL_FLOAT,// type
        GL_FALSE, // normalized?
        0, // stride
        (void*)0 // array buffer offset
        );

        // 1 - attribute buffer : UVs
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, drawCall->GetUVBufferID());
        glVertexAttribPointer(
            1,                                // attribute. No particular reason for 2, but must match the layout in the shader.
            2,                                // size : U+V --> 2
            GL_FLOAT,                         // type
            GL_FALSE,                         // normalized?
            0,                                // stride
            (void*)0                          // array buffer offset
        );
    }
}

void GLContext::Tick(glm::vec3 camPosition, glm::vec3 camDirection)
{
    for (int i = 0; i < drawCalls.size(); i++){
        CheckGLError("GL Context Tick");

        Log("Drawing ", i, "th draw call", " with VAO ", drawCalls[i]->GetVAO());
        glBindVertexArray(drawCalls[i]->GetVAO());
        
        CheckGLError("GL Bind Vertex Array");
        DrawCall* drawCall = drawCalls[i];

        glm::mat4 ProjectionMatrix;
        glm::mat4 ViewMatrix;

        //calculate MVP based on transformed camera
        ViewMatrix  = glm::lookAt(
            camPosition,           // Camera is here
            camPosition+camDirection, // and looks here : at the same camPosition, plus "camDirection"
            glm::vec3(0,1,0)                 // Head is up (set to 0,-1,0 to look upside-down)
        );

        // Projection matrix : 45&deg; Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
        ProjectionMatrix = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 1000.0f);
        glm::mat4 ModelMatrix = glm::mat4(1.0);
        glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;

        // Only during the initialisation
        MVPMatrixID = glGetUniformLocation(drawCall->GetProgramID(), "MVP");
        ViewMatrixID = glGetUniformLocation(drawCall->GetProgramID(), "V");
        ModelMatrixID = glGetUniformLocation(drawCall->GetProgramID(), "M");

        glUseProgram(drawCall->GetProgramID());

        CheckGLError("GL Use Program");

        // Send our transformation to the currently bound shader, in the "MVP" uniform
        // This is done in the main loop since each model will have a different MVP matrix (At least for the M part)
        glUniformMatrix4fv(MVPMatrixID, 1, GL_FALSE, &MVP[0][0]);
        glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
        glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &ViewMatrix[0][0]);

        CheckGLError("GL Uniform Matrix");

        if (drawCall->GetType() == DrawCallType::MESH){
            // Bind light too
            glUniform3f(drawCall->GetLightID(), 1, 4, 3);
            glUniform1f(drawCall->GetLightIntID(), 16.0f);
            CheckGLError("GL Uniforms Light");
        }

        //bind texture
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, drawCall->GetTextureID());
        glUniform1i(drawCall->GetTextureUniformID(), 0);

        CheckGLError("GL Texture Bind");

        SetAttribLayout(drawCall);

        // Draw the triangle !
        //glDrawArrays(GL_TRIANGLES, 0, drawCall->GetVertices().size() );
        //Log("Drawing ", drawCall->vboIndices.size(), " indices");

        if (drawCall->GetType() == DrawCallType::TEXT){
            glDrawArrays(GL_TRIANGLES, 0, drawCall->GetVertices().size());
        }else if (drawCall->GetType() == DrawCallType::MESH){
            glDrawElements(GL_TRIANGLES, drawCall->GetVBOIndices().size(), GL_UNSIGNED_INT, (void*)0);
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