#include "pet_app.hpp"
#include "pet_pipeline.hpp"

#include <iostream>
#include <cstdlib>
#include <stdexcept>

int main(){
    PetEngine3D::PetApp app;
    try{
        app.run();
    } catch (const std::exception& e){
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

namespace PetEngine3D{
    void PetApp::run(){
        while(!window.shouldClose()){
            glfwPollEvents();
        }
    }
}