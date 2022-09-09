#version 330 core

layout (location = 0) in vec3 aPos;     // the position variable has attribute position 0
layout (location = 1) in vec3 aColor;   // the color variable has attribute position 1
layout (location = 2) in vec3 aNormal;  // the normal variable has attribute position 2
layout (location = 3) in vec2 aTexCoord; // texture coordinates

uniform mat4 mvp;   // model view projection matrix
  
out vec3 ourColor;  // output a color to the fragment shader
out vec3 Normal;    // output normal vector
out vec2 TexCoord; // output textCoord

void main() {
    gl_Position = mvp * vec4(aPos, 1.0);    // Position
    ourColor = aColor;                      // set ourColor to the input color we got from the vertex data
    Normal = aNormal;                       // set normal to aNormal
    TexCoord = aTexCoord;
}