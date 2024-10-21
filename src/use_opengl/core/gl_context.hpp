#pragma once
#include <vector>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>
#include "drawcall.hpp"

class GLContext{
public:
    GLContext();
    ~GLContext();

    void Tick(glm::vec3 camPosition, glm::vec3 camDirection);

    void AddDrawCall(DrawCall* drawCall){drawCalls.push_back(drawCall);}
    
private:
    GLuint MVPMatrixID;
    GLuint ViewMatrixID;
    GLuint ModelMatrixID;

    std::vector<DrawCall*> drawCalls;
};