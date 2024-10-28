
#include "core/core.hpp"
void initText2D(const char * texturePath);
void printText2D(std::string_view text, int x, int y, int size, GLContext* context);
void cleanupText2D();