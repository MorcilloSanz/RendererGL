#version 330 core

out vec4 FragColor;

in vec3 ourColor;
in vec3 Normal;
in vec2 TexCoord;

uniform sampler2D tex;
uniform bool hasTexture;

void main() {
   vec4 color = vec4(ourColor, 1.0);
   color = hasTexture ? color * texture(tex, TexCoord).rgba : color;
   FragColor = color;
}