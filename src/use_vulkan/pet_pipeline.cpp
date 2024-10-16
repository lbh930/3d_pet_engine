#include "pet_pipeline.hpp"

#include <fstream>
#include <stdexcept>
#include <iostream>

namespace PetEngine3D{
    PetPipeline::PetPipeline(PetDevice& device,
        const std::string& vertFilepath,
        const std::string& fragFilepath,
        const PipelineConfigInfo& configInfo): device(device){
        
        createGraphicsPipeline(vertFilepath, fragFilepath, configInfo);

    }

    void PetPipeline::createShaderModule(const std::vector<char>& code, VkShaderModule* shaderModule){
        VkShaderModuleCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        createInfo.codeSize = code.size();

        //pointer to the bytecode
        createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

        if (vkCreateShaderModule(device.device(), &createInfo, nullptr, shaderModule) != VK_SUCCESS){
            throw std::runtime_error("Failed to create shader module!");
        }
    }

    PetPipeline::~PetPipeline(){
    }

    std::vector<char> PetPipeline::readFile(const std::string& filepath){
        std::ifstream file(filepath, std::ios::ate | std::ios::binary);

        if (!file.is_open()){
            throw std::runtime_error("Failed to open file: " + filepath);
        }

        size_t fileSize = static_cast<size_t>(file.tellg());

        std::vector<char> buffer(fileSize);

        file.seekg(0);
        file.read(buffer.data(), fileSize);

        file.close();

        return buffer;
    }

    PipelineConfigInfo PetPipeline::defaultPipelineConfigInfo(uint32_t width, uint32_t height){
        PipelineConfigInfo configInfo{};
        return configInfo;
    }

    void PetPipeline::createGraphicsPipeline(const std::string& vertFilepath, const std::string& fragFilepath, const PipelineConfigInfo& configInfo){
        auto vertCode = readFile(vertFilepath);
        auto fragCode = readFile(fragFilepath);

        std::cout << "Vertex Shader Code Size: " << vertCode.size() << std::endl;

        std::cout << "Fragment Shader Code Size: " << fragCode.size() << std::endl;
    }
}