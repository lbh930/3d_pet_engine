#include "gl_shader.hpp"
#include "gl_check.hpp"
#include "log/log.hpp"
#include <fstream>
#include <sstream>
#include <vector>
#include <stdexcept>

// Loads and compiles vertex and fragment shaders, then links them into an OpenGL program.
// Expects file paths for both shaders, compiles each, links them into a program, and returns the program ID.
// If any step fails, returns 0 and logs the error.
GLuint LoadShaders(const char* vertex_file_path, const char* fragment_file_path) {
    auto readShaderCode = [](const char* filePath) -> std::string {
        std::ifstream shaderStream(filePath, std::ios::in);
        if (!shaderStream.is_open()) {
            throw std::runtime_error(std::string("Impossible to open ") + filePath);
        }
        std::stringstream sstr;
        sstr << shaderStream.rdbuf();
        return sstr.str();
    };

    auto compileShader = [](const std::string& shaderCode, GLenum shaderType, const char* shaderName) -> GLuint {
        GLuint shaderID = glCreateShader(shaderType);
        if (shaderID == 0) {
            throw std::runtime_error(std::string("Failed to create shader: ") + shaderName);
        }

        const char* sourcePointer = shaderCode.c_str();
        glShaderSource(shaderID, 1, &sourcePointer, nullptr);
        glCompileShader(shaderID);

        GLint result = GL_FALSE;
        glGetShaderiv(shaderID, GL_COMPILE_STATUS, &result);
        if (result == GL_FALSE) {
            GLint infoLogLength;
            glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
            std::vector<char> errorMessage(infoLogLength + 1);
            glGetShaderInfoLog(shaderID, infoLogLength, nullptr, errorMessage.data());
            throw std::runtime_error(std::string(shaderName) + " Compilation Error: " + errorMessage.data());
        }

        return shaderID;
    };

    try {
        // Read shader code from files
        std::string vertexCode = readShaderCode(vertex_file_path);
        std::string fragmentCode = readShaderCode(fragment_file_path);

        // Compile shaders
        Log("Compiling shader: ", vertex_file_path);
        GLuint vertexShaderID = compileShader(vertexCode, GL_VERTEX_SHADER, "Vertex Shader");

        Log("Compiling shader: ", fragment_file_path);
        GLuint fragmentShaderID = compileShader(fragmentCode, GL_FRAGMENT_SHADER, "Fragment Shader");

        // Link the program
        Log("Linking program");
        GLuint programID = glCreateProgram();
        if (programID == 0) {
            throw std::runtime_error("Failed to create program object.");
        }

        glAttachShader(programID, vertexShaderID);
        glAttachShader(programID, fragmentShaderID);
        glLinkProgram(programID);

        // Check the program
        GLint result = GL_FALSE;
        glGetProgramiv(programID, GL_LINK_STATUS, &result);
        if (result == GL_FALSE) {
            GLint infoLogLength;
            glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &infoLogLength);
            std::vector<char> errorMessage(infoLogLength + 1);
            glGetProgramInfoLog(programID, infoLogLength, nullptr, errorMessage.data());
            throw std::runtime_error(std::string("Program Linking Error: ") + errorMessage.data());
        }

        glDetachShader(programID, vertexShaderID);
        glDetachShader(programID, fragmentShaderID);
        glDeleteShader(vertexShaderID);
        glDeleteShader(fragmentShaderID);

        Log("Shaders loaded and program linked successfully. Program ID: ", programID);
        return programID;

    } catch (const std::exception& e) {
        Error("Shader loading failed: ", e.what());
        return 0;
    }
}
