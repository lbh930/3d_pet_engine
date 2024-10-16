#version 450 core

layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec3 vertexColor;

out vec3 fragmentColor; // 输出到片段着色器的颜色

uniform mat4 MVP;

void main(){
  gl_Position =  MVP * vec4(vertexPosition_modelspace, 1);
  fragmentColor = vertexColor; // 将顶点颜色传递到片段着色器
}
