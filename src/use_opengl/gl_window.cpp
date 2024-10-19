#include <iostream>
#include <cstdlib>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "common/gl_shader.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include "common/bmp_loader.hpp"
#include <cmath>
#include "obj_loader.hpp"

glm::mat4 ProjectionMatrix;
glm::mat4 ViewMatrix;
glm::vec3 position = glm::vec3(0.0f,0.0f,5.0f);
float horizontal = 3.14f;
float vertical = 0;
glm::vec3 direction = glm::vec3(0.0f,0.0f,-1.0f);
float speed = 3.0f;
float mouseSpeed = 0.1f;

int main(){
    glewExperimental = GL_TRUE; //for core profile

    if (!glfwInit()){
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return EXIT_FAILURE;
    }

    //set anti-aliasing to 4x
    glfwWindowHint(GLFW_SAMPLES, 4);

    // Use OpenGL 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    // To make MacOS happy; should not be needed
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // We don't want the old OpenGL 
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); 

    GLFWwindow* window = glfwCreateWindow(800, 600, "Laolu Productions", NULL, NULL);

    if (window == NULL){
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return EXIT_FAILURE;
    }
    
    glfwMakeContextCurrent(window);

    glfwMakeContextCurrent(window); 
    glewExperimental = true; // Needed in core profile
    // Initialize GLEW
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        return -1;
    }

    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    //load the obj model
    std::vector< glm::vec3 > vertices;
    std::vector< glm::vec2 > uvs;
    std::vector< glm::vec3 > normals; // Won't be used at the moment.
    bool res = loadOBJ("../../objs/ring.obj", vertices, uvs, normals);

    // Buffer vertices and UVs
    GLuint vertexbuffer;
    GLuint uvbuffer;
    glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);
    std::cout<<"Vertex Buffer Data Size: "<<vertices.size() * sizeof(glm::vec3)<<std::endl;
    glGenBuffers(1, &uvbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
    glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);
    std::cout<<"UV Buffer Data Size: "<<uvs.size()*sizeof(glm::vec2)<<std::endl;

    // Create and compile GLSL program from the shaders
    GLuint programID = LoadShaders( "../../shaders/vertex.glsl", "../../shaders/fragment.glsl" );

    //loadTexture
    GLuint Texture = loadBMP("../../textures/sampletexture.bmp");
    GLuint TextureID = glGetUniformLocation(programID, "myTextureSampler");
    glUniform1i(TextureID, 0);


    // Projection matrix: 45° Field of View, 4:3 ratio, display range: 0.1 unit <-> 100 units
    glm::mat4 Projection = glm::perspective(glm::radians(45.0f), (float) 800 / (float)600, 0.1f, 100.0f);

    // Get a handle for our "MVP" uniform
    // Only during the initialisation
    GLuint MatrixID = glGetUniformLocation(programID, "MVP");

    static double lastTime = glfwGetTime();


    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    // Accept fragment if it closer to the camera than the former one

    glEnable(GL_CULL_FACE); // Cull triangles which normal is not towards the camera

    glDepthFunc(GL_LESS);

    do{
        double currentTime = glfwGetTime();
        float deltaTime = float(currentTime - lastTime);
        lastTime = currentTime;

        //std::cout<<deltaTime<<std::endl;

        // Get mouse position
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);

        //std::cout<<"Mouse Position: "<<xpos<<", "<<ypos<<std::endl;

        // Reset mouse position for next frame
        glfwSetCursorPos(window, 800.0f/2.0f, 600.0f/2.0f);

        // Compute new orientation
        horizontal += mouseSpeed * deltaTime * float(800/2 - xpos );
        vertical   += mouseSpeed * deltaTime * float( 600/2 - ypos );

        // Direction : Spherical coordinates to Cartesian coordinates conversion
        glm::vec3 direction(
            cos(vertical) * sin(horizontal),
            sin(vertical),
            cos(vertical) * cos(horizontal)
        );

        //std::cout<<"Direction: "<<direction.x<<", "<<direction.y<<", "<<direction.z<<std::endl;
        //std::cout<<position.x<<", "<<position.y<<", "<<position.z<<std::endl;

        // Right vector
        glm::vec3 right = glm::vec3(
            sin(horizontal - 3.14f/2.0f),
            0,
            cos(horizontal - 3.14f/2.0f)
        );

        // Move forward
        if (glfwGetKey(window,  GLFW_KEY_W ) == GLFW_PRESS){
            position += direction * deltaTime * speed;
        }
        // Move backward
        if (glfwGetKey(window,  GLFW_KEY_S ) == GLFW_PRESS){
            position -= direction * deltaTime * speed;
        }
        // Strafe right
        if (glfwGetKey(window, GLFW_KEY_D ) == GLFW_PRESS){
            position += right * deltaTime * speed;
        }
        // Strafe left
        if (glfwGetKey(window, GLFW_KEY_A ) == GLFW_PRESS){
            position -= right * deltaTime * speed;
        }

        //calculate MVP based on transformed camera
        ViewMatrix  = glm::lookAt(
            position,           // Camera is here
            position+direction, // and looks here : at the same position, plus "direction"
            glm::vec3(0,1,0)                 // Head is up (set to 0,-1,0 to look upside-down)
        );

        // Projection matrix : 45&deg; Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
        ProjectionMatrix = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
        glm::mat4 ModelMatrix = glm::mat4(1.0);
        glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;

        // Send our transformation to the currently bound shader, in the "MVP" uniform
        // This is done in the main loop since each model will have a different MVP matrix (At least for the M part)
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

        // Clear the screen. It's not mentioned before Tutorial 02, but it can cause flickering, so it's there nonetheless.
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(programID);

        //bind texture
        glActiveTexture(GL_TEXTURE0);  // 激活纹理单元 0
        glBindTexture(GL_TEXTURE_2D, Texture);  // 将加载的纹理绑定到 GL_TEXTURE_2D


        // Draw
        // 0 - attribute buffer : vertices
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glVertexAttribPointer(
        0,// attribute 0. No particular reason for 0, but must match the layout in the shader.
        3, // size
        GL_FLOAT,// type
        GL_FALSE, // normalized?
        0, // stride
        (void*)0 // array buffer offset
        );
/*
        // 1 - attribute buffer : colors
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
        glVertexAttribPointer(
            1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
            3,                                // size
            GL_FLOAT,                         // type
            GL_FALSE,                         // normalized?
            0,                                // stride
            (void*)0                          // array buffer offset
        );
*/
        // 2 - attribute buffer : UVs
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
        glVertexAttribPointer(
            1,                                // attribute. No particular reason for 2, but must match the layout in the shader.
            2,                                // size : U+V --> 2
            GL_FLOAT,                         // type
            GL_FALSE,                         // normalized?
            0,                                // stride
            (void*)0                          // array buffer offset
        );

        // Draw the triangle !
        glDrawArrays(GL_TRIANGLES, 0, 12*3); // 12*3 indices starting at 0 -> 12 triangles -> 6 squares
        glDisableVertexAttribArray(0);

        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();

    } // Check if the ESC key was pressed or the window was closed
    while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
        glfwWindowShouldClose(window) == 0 );

}