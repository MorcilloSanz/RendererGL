#version 330 core

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;    
    float shininess;
}; 

struct Light {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 ourColor;
in vec3 Normal;  
in vec3 FragPos;  
in vec2 TexCoord;

out vec4 FragColor;
  
uniform vec3 viewPos;
uniform Material material;
uniform Light light;

uniform sampler2D textures[64];
uniform int nTextures;

void main() {

    // Ambient
    vec3 ambient = light.ambient * ourColor;
  	
    // Diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * (diff * material.diffuse);
    
    // Specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * (spec * material.specular);  
        
    // Output
    vec3 result = (ambient + diffuse + specular) * ourColor;

    // Texture interpolation
    vec4 color = vec4(result, 1.0);
    for(int i = 0; i < nTextures; i ++) {
        color = color * vec4(texture(textures[i], TexCoord).rgb, 1.0);
    }
    FragColor = color;
} 