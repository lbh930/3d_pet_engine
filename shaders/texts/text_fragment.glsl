
#version 330 core

uniform sampler2D myTextureSampler;

out vec4 color;

in vec2 UV;

void main(){
    color = texture(myTextureSampler, UV).rgba;
}