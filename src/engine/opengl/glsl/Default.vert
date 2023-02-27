#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec3 aNormal;
layout (location = 3) in vec2 aTexCoord;

uniform mat4 mvp;

out vec3 ourColor;
out vec3 Normal;
out vec2 TexCoord;

void main() {
    gl_Position = mvp * vec4(aPos, 1.0);
    ourColor = aColor;
    Normal = aNormal;
    TexCoord = aTexCoord;
}