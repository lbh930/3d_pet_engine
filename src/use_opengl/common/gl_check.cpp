#include "gl_check.hpp"
#include "GL/glew.h"
#include <iostream>
#include <unordered_map>
#include "log/log.hpp"

// Checks for any OpenGL errors after specific calls.
// Logs errors to the console with a context string for easy tracking.
void CheckGLError(const std::string& s) {
    GLenum err;
    std::unordered_map<GLenum, std::string> errorMap = {
        {GL_INVALID_ENUM, "GL_INVALID_ENUM"},
        {GL_INVALID_VALUE, "GL_INVALID_VALUE"},
        {GL_INVALID_OPERATION, "GL_INVALID_OPERATION"},
        {GL_STACK_OVERFLOW, "GL_STACK_OVERFLOW"},
        {GL_STACK_UNDERFLOW, "GL_STACK_UNDERFLOW"},
        {GL_OUT_OF_MEMORY, "GL_OUT_OF_MEMORY"},
        {GL_INVALID_FRAMEBUFFER_OPERATION, "GL_INVALID_FRAMEBUFFER_OPERATION"},
        // todo: add more error codes
    };

    bool hasError = false;

    while ((err = glGetError()) != GL_NO_ERROR) {
        hasError = true;
        std::string errorString = errorMap.count(err) ? errorMap[err] : "Unknown Error";
        Error(s, " OpenGL error: ", errorString, " (", err, ")");
    }

    if (!hasError) {
        Log("OpenGL no error: ", s);
    }
}
