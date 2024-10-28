#pragma once
#include <GL/glew.h>
#include <vector>

bool loadBMP(const char* imagePath, std::vector<unsigned char> &data, std::pair<int, int>& textureSize);