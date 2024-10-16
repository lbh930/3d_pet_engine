#include "pet_window.hpp"
#include <stdexcept>
namespace PetEngine3D{
    PetWindow::PetWindow(int width, int height, std::string windowName) : width(width), height(height), windowName(windowName){
        initWindow();
    }

    void PetWindow::initWindow(){
        glfwInit();

        //GLFW_NO_API tells GLFW to not create an OpenGL context
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

        window = glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr);
    }

    void PetWindow::createWindowSurface(VkInstance instance, VkSurfaceKHR* surface){
        if (glfwCreateWindowSurface(instance, window, nullptr, surface) != VK_SUCCESS){
            throw std::runtime_error("Failed to create window surface!");
        }
    }

    PetWindow::~PetWindow(){
        glfwDestroyWindow(window);
        glfwTerminate();
    }
}