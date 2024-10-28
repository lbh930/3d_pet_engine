#include "bmp_loader.hpp"
#include <iostream>
#include <fstream>
#include <vector>
#include <array>
#include <string>
#include "log/log.hpp"
#include <GL/glew.h>

// Loads a BMP image and creates an OpenGL texture from it.
// Takes the file path of the BMP image.
// Returns the OpenGL texture ID if loading was successful, or 0 if it failed.
bool loadBMP(const char* imagePath, std::vector<unsigned char> &data, std::pair<int, int>& textureSize) {
    constexpr size_t headerSize = 54;  // Each BMP file begins with a 54-byte header
    std::array<unsigned char, headerSize> header{};
    unsigned int dataPos;
    unsigned int width, height;
    unsigned int imageSize;

    // Open the file
    std::ifstream file(imagePath, std::ios::binary);
    if (!file) {
        printf("Image could not be opened\n");
        return 0;
    }

    // Read the header
    if (!file.read(reinterpret_cast<char*>(header.data()), headerSize)) {
        printf("Not a correct BMP file\n");
        return 0;
    }

    // Check the BMP signature
    if (header[0] != 'B' || header[1] != 'M') {
        printf("Not a correct BMP file\n");
        return 0;
    }

    // Read integers from the byte array (little-endian format)
    auto readUInt32 = [](const unsigned char* buffer, size_t offset) -> unsigned int {
        return static_cast<unsigned int>(buffer[offset]) |
            (static_cast<unsigned int>(buffer[offset + 1]) << 8) |
            (static_cast<unsigned int>(buffer[offset + 2]) << 16) |
            (static_cast<unsigned int>(buffer[offset + 3]) << 24);
    };

    dataPos = readUInt32(header.data(), 0x0A);
    imageSize = readUInt32(header.data(), 0x22);
    width = readUInt32(header.data(), 0x12);
    height = readUInt32(header.data(), 0x16);

    textureSize = std::make_pair(width, height);

    // Some BMP files are misformatted; guess missing information
    if (imageSize == 0) imageSize = width * height * 3;  // 3 bytes per pixel (RGB)
    if (dataPos == 0) dataPos = headerSize;

    // Create a buffer
    data = std::vector<unsigned char>(imageSize);

    // Read the actual data from the file into the buffer
    file.seekg(dataPos, std::ios::beg); //set the file read pointer to the beginning of the image data
    if (!file.read(reinterpret_cast<char*>(data.data()), imageSize)) {
        printf("Failed to read image data\n");
        return 0;
    }

    // Close the file
    file.close();

    Log("Texture loaded: ", imagePath);

    return true;
}
