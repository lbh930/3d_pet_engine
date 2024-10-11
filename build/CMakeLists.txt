cmake_minimum_required(VERSION 3.10)
project(3DPetEngine)

# 设置C++标准
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 设置源文件和头文件路径
file(GLOB_RECURSE SOURCES "../src/*.cpp" "../src/*.hpp")

# 添加可执行文件
add_executable(${PROJECT_NAME} ${SOURCES})

# 设置库的包含目录
include_directories(
    ../lib/glm
    ../lib/glfw-3.4.bin.WIN64/include
    ../lib/Vulkan/Include
)

# 链接库
target_link_libraries(${PROJECT_NAME}
    ../lib/Vulkan/Lib/vulkan-1
    ../lib/glfw-3.4.bin.WIN64/lib-vc2022/glfw3
)

# 如果有其他需要配置的选项或定义，可以在这里添加
# target_compile_definitions(${PROJECT_NAME} PRIVATE MY_DEFINE)
