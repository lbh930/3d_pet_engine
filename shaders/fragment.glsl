#version 450 core

in vec3 fragmentColor;

out vec3 color;

void main(){
    // Output color = color specified in the vertex shader,
    // interpolated between all 3 surrounding vertices
    color = fragmentColor;
}