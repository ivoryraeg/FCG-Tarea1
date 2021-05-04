#version 330 core
layout(location = 0) in vec3 vertexPosition_modelspace;
// Notice that the "1" here equals the "1" in glVertexAttribPointer
//layout(location = 1) in vec3 vertexColor;

// Values that stay constant for the whole mesh.
uniform mat4 MVP;

void main(){
    gl_Position =  MVP * vec4(vertexPosition_modelspace*0.4,1);
}