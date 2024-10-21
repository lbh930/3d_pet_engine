#include "gl_check.hpp"
#include "GL/glew.h"
#include <iostream>
#include <thread> // 包含 sleep_for 函数
#include <chrono> // 包含时间单位

void CheckGLError(std::string s){
    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR) {
        std::cerr << "GL Error!" << s << ": " << err << std::endl;

        std::this_thread::sleep_for(std::chrono::seconds(2));

        std::cout << "Sleep finished." << std::endl;
    }
    
}