#version 330 core
// Output data ; will be interpolated for each fragment.
//out vec3 fragmentColor = vec3(1,1,1);
// Interpolated values from the vertex shaders
//in vec3 fragmentColor = vec3(1,1,1);
// Ouput data
out vec3 color;

uniform vec3 ourColor = vec3(1,1,1);

void main(){
    
  color = ourColor;//No borrar
  //fragmentColor = vertexColor;
  //color = fragmentColor;
}
