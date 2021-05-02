#version 330 core
layout(location = 0) in vec3 vertexPosition_modelspace;
// Notice that the "1" here equals the "1" in glVertexAttribPointer
//layout(location = 1) in vec3 vertexColor;

void main(){
    gl_Position.xyz = vertexPosition_modelspace*0.1;
    gl_Position.w = 1.0;
}