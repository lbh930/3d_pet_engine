#pragma once
#include "pet_window.hpp"
#include "pet_pipeline.hpp"
#include "pet_device.hpp"

namespace PetEngine3D{

    class PetApp{
    public:
        static constexpr int WIDTH = 800;
        static constexpr int HEIGHT = 800;

        void run();

    private:
        PetWindow window{WIDTH, HEIGHT, "LaoluProductions"};
        PetDevice device{window};
        PetPipeline pipeline {device, "../../shaders/simple_shader.vert.spv", "../../shaders/simple_shader.frag.spv", PetPipeline::defaultPipelineConfigInfo(WIDTH, HEIGHT)};
    };
}