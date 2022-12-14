#pragma once

#include <iostream>

class Program {
public:
    static std::string getVertexShaderCode() {
        return
        "#version 330 core\n"
        "\n"
        "layout (location = 0) in vec3 aPos;\n"
        "layout (location = 1) in vec3 aColor;\n"
        "layout (location = 2) in vec3 aNormal;\n"
        "layout (location = 3) in vec2 aTexCoord;\n"
        "\n"
        "uniform mat4 mvp;\n"
        "\n"
        "out vec3 ourColor;\n"
        "out vec3 Normal;\n"
        "out vec2 TexCoord;\n"
        "\n"
        "void main() {\n"
        "    gl_Position = mvp * vec4(aPos, 1.0);\n"
        "    ourColor = aColor;\n"
        "    Normal = aNormal;\n"
        "    TexCoord = aTexCoord;\n"
        "}\n";
    };

    static std::string getFragmentShaderCode() {
        return
        "#version 330 core\n"
        "\n"
        "out vec4 FragColor;\n"
        "\n"
        "in vec3 ourColor;\n"
        "in vec3 Normal;\n"
        "in vec2 TexCoord;\n"
        "\n"
        "uniform sampler2D textures[64];\n"
        "uniform int nTextures;\n"
        "\n"
        "void main() {\n"
        "   vec4 color = vec4(ourColor, 1.0);\n"
        "   for(int i = 0; i < nTextures; i ++) {\n"
        "       color = color * vec4(texture(textures[i], TexCoord).rgb, 1.0);\n"
        "   }\n"
        "   FragColor = color;\n"
        "}\n";
    }

    static std::string getPhongVertexShaderCode() {
        return
        "#version 330 core\n"
        "\n"
        "layout (location = 0) in vec3 aPos;\n"
        "layout (location = 1) in vec3 aColor;\n"
        "layout (location = 2) in vec3 aNormal;\n"
        "layout (location = 3) in vec2 aTexCoord;\n"
        "\n"
        "out vec3 FragPos;\n"
        "out vec3 ourColor;\n"
        "out vec3 Normal;\n"
        "out vec2 TexCoord;\n"
        "\n"
        "uniform mat4 model;\n"
        "uniform mat4 view;\n"
        "uniform mat4 projection;\n"
        "\n"
        "void main() {\n"
        "   FragPos = vec3(model * vec4(aPos, 1.0));\n"
        "   ourColor = aColor;\n"
        "   Normal = mat3(transpose(inverse(model))) * aNormal;\n"
        "   TexCoord = aTexCoord;\n"
        "   gl_Position = projection * view * vec4(FragPos, 1.0);\n"
        "}\n";
    };

    static std::string getPhongFragmentShaderCode() {
        return
        "#version 330 core\n"
        "\n"
        "#define MAX_LIGHTS 64\n"
        "#define MAX_TEXTURES 64\n"
        "\n"
        "struct Material {\n"
        "    vec3 ambient;\n"
        "    vec3 diffuse;\n"
        "    vec3 specular;\n" 
        "    float shininess;\n"
        "};\n" 
        "\n"
        "struct Light {\n"
        "    vec3 position;\n"
        "    vec3 color;\n"
        "    vec3 ambient;\n"
        "    vec3 diffuse;\n"
        "    vec3 specular;\n"
        "};\n"
        "\n"
        "in vec3 ourColor;\n"
        "in vec3 Normal;\n"
        "in vec3 FragPos;\n"
        "in vec2 TexCoord;\n"
        "\n"
        "out vec4 FragColor;\n"
        "\n"
        "uniform vec3 viewPos;\n"
        "uniform Material material;\n"
        "\n"
        "uniform Light lights[MAX_LIGHTS];\n"
        "uniform int nLights;"
        "\n"
        "uniform sampler2D textures[MAX_TEXTURES];\n"
        "uniform int nTextures;\n"
        "\n"
        "uniform bool blinn;\n"
        "uniform bool gammaCorrection;\n"
        "\n"
        "vec3 getLightColor(Light light) {\n"
        "\n"
        "    vec3 ambient = light.ambient * light.color;\n"
        "\n"
        "    vec3 norm = normalize(Normal);\n"
        "    vec3 lightDir = normalize(light.position - FragPos);\n"
        "    float diff = max(dot(norm, lightDir), 0.0);\n"
        "    vec3 diffuse = light.diffuse * (diff * material.diffuse);\n"
        "\n"    
        "    vec3 viewDir = normalize(viewPos - FragPos);\n"
        "    vec3 reflectDir = reflect(-lightDir, norm);\n"
        "    float spec = 0.0;\n"
        "    if(blinn) {\n"
        "       vec3 halfwayDir = normalize(lightDir + viewDir); \n"
        "       spec = pow(max(dot(norm, halfwayDir), 0.0), material.shininess);\n"
        "    } else {\n"
        "       spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);\n"
        "    }\n"
        "    vec3 specular = light.specular * (spec * material.specular);\n"
        "\n"
        "    vec3 result = (ambient + diffuse + specular) * ourColor;\n"
        "    return result;\n"
        "}\n"
        "\n"
        "void main() {\n"
        "    vec3 lightColor = ourColor;\n"
        "    for(int i = 0; i < nLights; i ++) {\n"
        "       lightColor += getLightColor(lights[i]);\n"
        "    }\n"
        "\n"
        "    vec4 color = vec4(lightColor, 1.0);\n"
        "    for(int i = 0; i < nTextures; i ++) {\n"
        "        color = color * vec4(texture(textures[i], TexCoord).rgb, 1.0);\n"
        "    }\n"
        "    FragColor = color;\n"
        "\n"
        "    if(gammaCorrection) {\n"
        "       float gamma = 2.2;\n"
        "       FragColor.rgb = pow(FragColor.rgb, vec3(1.0/gamma));\n"
        "    }\n"
        "}\n";
    }

    static std::string getSkyBoxVertexShaderCode() {
        return
        "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "\n"
        "out vec3 TexCoords;\n"
        "\n"
        "uniform mat4 projection;\n"
        "uniform mat4 view;\n"
        "\n"
        "void main()\n"
        "{\n"
        "    TexCoords = aPos;\n"
        "    vec4 pos = projection * view * vec4(aPos, 1.0);\n"
        "    gl_Position = pos.xyzw;\n"
        "}\n";
    }

    static std::string getSkyBoxFragmentShaderCode() {
        return
        "#version 330 core\n"
        "out vec4 FragColor;\n"
        "\n"
        "in vec3 TexCoords;\n"
        "\n"
        "uniform samplerCube skybox;\n"
        "\n"
        "void main()\n"
        "{\n"
        "    FragColor = texture(skybox, TexCoords);\n"
        "}\n";
    }

    static std::string getOutlineVertexShaderCode() {
        return
        "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "\n"
        "uniform mat4 mvp;\n"
        "\n"
        "void main()\n"
        "{\n"
        "    vec4 pos = mvp * vec4(aPos, 1.0);\n"
        "    gl_Position = pos.xyzw;\n"
        "}\n";
    }

    static std::string getOutlineFragmentShaderCode() {
        return
        "#version 330 core\n"
        "out vec4 FragColor;\n"
        "\n"
        "void main() {\n"
        "    FragColor = vec4(1.0, 0.7, 0.28, 1.0);\n"
        "}\n";
    }
};