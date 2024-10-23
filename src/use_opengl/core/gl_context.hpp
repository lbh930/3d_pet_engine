#pragma once
#include <vector>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>
#include "drawcall.hpp"
#include <tuple>

class GLContext{
public:
    GLContext();
    ~GLContext();

    void Tick(glm::vec3 camPosition, glm::vec3 camDirection);

    void AddDrawCall(DrawCall* drawCall){drawCalls.push_back(drawCall);}

    void ClearDrawCalls();

    void SetResolution(int x, int y){resolutionX = x; resolutionY = y;}

    std::tuple<int, int> GetResolution(){return std::make_tuple(resolutionX, resolutionY);}
    
private:
    GLuint MVPMatrixID;
    GLuint ViewMatrixID;
    GLuint ModelMatrixID;

    std::vector<DrawCall*> drawCalls;

    int resolutionX;
    int resolutionY;
};