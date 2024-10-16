#pragma once
#include <string>
#include <vector>

#include "pet_device.hpp"

namespace PetEngine3D{
    struct PipelineConfigInfo{};

    class PetPipeline{
    public:
        PetPipeline(PetDevice& device,
            const std::string& vertFilepath,
            const std::string& fragFilepath,
            const PipelineConfigInfo& configInfo);

        ~PetPipeline();

        PetPipeline(const PetPipeline&) = delete;
        PetPipeline& operator=(const PetPipeline&) = delete;

        static PipelineConfigInfo defaultPipelineConfigInfo(uint32_t width, uint32_t height);

    private:
        static std::vector<char> readFile (const std::string& filepath);

        void createGraphicsPipeline(const std::string& vertFilepath,
            const std::string& fragFilepath,
            const PipelineConfigInfo& configInfo);

        void createShaderModule(const std::vector<char>& code, VkShaderModule* shaderModule);

        PetDevice& device;
        VkPipeline graphicsPipeline;
        VkShaderModule vertShaderModule;
        VkShaderModule fragShaderModule;
    };
}