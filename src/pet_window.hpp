#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <string>

namespace PetEngine3D {
    class PetWindow{
    public:
        PetWindow(int width, int height, std::string windowName);
        ~PetWindow();

        bool shouldClose(){return glfwWindowShouldClose(window);}

        PetWindow(const PetWindow&) = delete;
        PetWindow& operator=(const PetWindow&) = delete;
        void createWindowSurface(VkInstance instance, VkSurfaceKHR* surface);

    private:
        void initWindow();
        const int width;
        const int height;
        std::string windowName;
        GLFWwindow* window;

    };
}