#include "gl_check.hpp"
#include "GL/glew.h"
#include <iostream>
#include <thread> // 包含 sleep_for 函数
#include <chrono> // 包含时间单位
#include "log/log.hpp"

void CheckGLError(std::string s){
    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR) {
        Error(s, "OpenGL error: ", err);

        std::this_thread::sleep_for(std::chrono::seconds(2));
    }
}