#include <iostream>
#include <cstdlib>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "common/gl_shader.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include "common/bmp_loader.hpp"
#include <cmath>
#include "common/obj_loader.hpp"
#include "core/gl_context.hpp"
#include "core/drawcall.hpp"
#include "log/log.hpp"
#include "common/gl_check.hpp"
#include "windows.h"
#include "text/text_draw.hpp"

glm::vec3 position = glm::vec3(0.0f,0.0f,5.0f);
float horizontal = 3.14f;
float vertical = 0;
glm::vec3 direction = glm::vec3(0.0f,0.0f,-1.0f);
float speed = 3.0f;
float mouseSpeed = 0.5f;

double lastCursorX = 800.0f/2.0f;
double lastCursorY = 600.0f/2.0f;

int ResolutionX = 800;
int ResolutionY = 600;

int main(){
    LogClearAll();

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

    // Create a windowed mode window and its OpenGL context
    glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
    glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);
    glfwWindowHint(GLFW_RED_BITS, 8);
    glfwWindowHint(GLFW_GREEN_BITS, 8);
    glfwWindowHint(GLFW_BLUE_BITS, 8);
    glfwWindowHint(GLFW_ALPHA_BITS, 8);

    GLFWwindow* window = glfwCreateWindow(ResolutionX, ResolutionY, "Laolu Productions", NULL, NULL);

    if (window == NULL){
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return EXIT_FAILURE;
    }
    
    glfwMakeContextCurrent(window); 
    glewExperimental = true; // Needed in core profile
    // Initialize GLEW
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        return -1;
    }

    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    CheckGLError("GLFW Init");

    //GL context creation
    GLContext context;
    context.SetResolution(ResolutionX, ResolutionY);

    //add draw call for the ring
    
    DrawCall drawCall;
    drawCall.BindProgramID(LoadShaders("shaders/vertex.glsl", "shaders/fragment.glsl"));
    CheckGLError("Shader Load");
    drawCall.AddLight(glm::vec3(1,4,2), glm::vec3(1,1,1));
    CheckGLError("Light Add");
    drawCall.SetType(DrawCallType::MESH);
    drawCall.AddModel("objs/ring.obj");
    CheckGLError("Model Add");
    drawCall.AddTexture("textures/ring.bmp");
    CheckGLError("Texture Add");
    drawCall.BufferInit();
    CheckGLError("Buffer Init");
    context.AddDrawCall(&drawCall);

    //show text
    printText2D("Hello World", 10, 10, 20, &context);

    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    // Accept fragment if it closer to the camera than the former one

    glEnable(GL_CULL_FACE); // Cull triangles which normal is not towards the camera

    // Enable blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glDepthFunc(GL_LESS);

    CheckGLError("GL Context Init");

    static double lastTime = glfwGetTime();

    float distance = glm::length(position);

    //intialize cursor pos
    glfwGetCursorPos(window, &lastCursorX, &lastCursorY);
    do{

        // Clear the screen
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        
        double currentTime = glfwGetTime();
        float deltaTime = float(currentTime - lastTime);
        lastTime = currentTime;

        //std::cout<<deltaTime<<std::endl;

        // Get mouse position
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);

        //std::cout<<"Mouse Position: "<<xpos<<", "<<ypos<<std::endl;

        // Reset mouse position for next frame
        //glfwSetCursorPos(window, 800.0f/2.0f, 600.0f/2.0f);

        // Right vector
        /*glm::vec3 right = glm::vec3(
            sin(horizontal - 3.14f/2.0f),
            0,
            cos(horizontal - 3.14f/2.0f)
        );*/

        //for now force look at origin
        direction = glm::vec3(0.0f,0.0f,0.0f) - position;

        glm::vec3 right = glm::normalize(glm::cross(glm::vec3(0.0f,1.0f,0.0f), direction));
        glm::vec3 up = glm::normalize(glm::cross( right, direction ));

        // Compute new orientation
        //horizontal += mouseSpeed * deltaTime * float(xpos - lastCursorX);
        //vertical   += mouseSpeed * deltaTime * float(ypos - lastCursorY);
        //Mouse control view as well

        position += -1.0f * right * float(xpos - lastCursorX) * deltaTime * mouseSpeed;
        position += up * float(ypos - lastCursorY) * deltaTime * mouseSpeed;
        position = glm::normalize(position) * distance;
        
        lastCursorX = xpos;
        lastCursorY = ypos;

        //for now force look at origin
        direction = glm::vec3(0.0f,0.0f,0.0f) - position;

        //std::cout<<"Direction: "<<direction.x<<", "<<direction.y<<", "<<direction.z<<std::endl;
        //std::cout<<position.x<<", "<<position.y<<", "<<position.z<<std::endl;
        // Move forward
        if (glfwGetKey(window,  GLFW_KEY_W ) == GLFW_PRESS){
            //position += direction * deltaTime * speed;
        }
        // Move backward
        if (glfwGetKey(window,  GLFW_KEY_S ) == GLFW_PRESS){
            //position -= direction * deltaTime * speed;
        }
        // Strafe right
        if (glfwGetKey(window, GLFW_KEY_D ) == GLFW_PRESS){
            //position += right * deltaTime * speed;
        }
        // Strafe left
        if (glfwGetKey(window, GLFW_KEY_A ) == GLFW_PRESS){
            //position -= right * deltaTime * speed;
        }

        // Direction : Spherical coordinates to Cartesian coordinates conversion
       /* glm::vec3 direction(
            cos(vertical) * sin(horizontal),
            sin(vertical),
            cos(vertical) * cos(horizontal)
        );*/


        //tick GL context to render
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        //Log("Position: ", position.x, ", ", position.y, ", ", position.z);
        //Log("Direction: ", direction.x, ", ", direction.y, ", ", direction.z);

        context.Tick(position, direction);

        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();

    } // Check if the ESC key was pressed or the window was closed
    while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
        glfwWindowShouldClose(window) == 0 );

}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, int nShowCmd) {
    return main();
}