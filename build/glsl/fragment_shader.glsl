#version 330 core

out vec4 FragColor;  

in vec3 ourColor;
in vec3 Normal;
in vec2 TexCoord;

uniform sampler2D textures[64];
uniform int nTextures;

void main() {
    
    vec4 color = vec4(ourColor, 1.0);
    for(int i = 0; i < nTextures; i ++) {
        color = color * vec4(texture(textures[i], TexCoord).rgb, 1.0);
    }
    FragColor = color;
}