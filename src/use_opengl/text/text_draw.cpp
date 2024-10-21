#include "text_draw.hpp"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>

void printText2D(const char * text, int x, int y, int size){
    unsigned int length = strlen(text);
    for (unsigned int i = 0; i < length; i++){
        glm::vec2 vertex_up_left = glm::vec2(x+i*size, y+size);
        glm::vec2 vertex_up_right = glm::vec2(x+i*size+size, y+size);
        glm::vec2 vertex_down_right = glm::vec2(x+i*size+size, y);
        glm::vec2 vertex_down_left = glm::vec2(x+i*size, y);
    }
}