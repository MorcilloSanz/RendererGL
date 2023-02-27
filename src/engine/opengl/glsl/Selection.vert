#version 330 core

layout (location = 0) in vec3 aPos;

uniform mat4 mvp;

void main() {
    vec4 pos = mvp * vec4(aPos, 1.0);
    gl_Position = pos.xyzw;
}