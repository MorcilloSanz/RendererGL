#version 330 core

#define MAX_LIGHTS 64
#define MAX_TEXTURES 64

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
}; 

struct Light {
    vec3 position;
    vec3 color;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;

    vec3 direction;
    float cutOff;
    float outerCutOff;
};

struct MaterialMaps {
   sampler2D diffuseMap;
   sampler2D specularMap;
   sampler2D emissionMap;
};

in vec3 ourColor;
in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoord;

out vec4 FragColor;

uniform vec3 viewPos;
uniform Material material;

uniform Light lights[MAX_LIGHTS];
uniform int nLights;
uniform bool isPointLight;
uniform bool isSpotLight;

uniform MaterialMaps materialMaps;
uniform bool hasDiffuse;
uniform bool hasSpecular;
uniform bool hasEmission;

uniform bool blinn;
uniform bool gammaCorrection;

vec3 getLightColor(Light light) {

   // Calculate ambient lighting
   vec3 ambient = vec3(1.0);
   if(hasDiffuse) {
      ambient = light.ambient * light.color * vec3(texture(materialMaps.diffuseMap, TexCoord));
   }else {
      ambient = light.ambient * light.color * ourColor;
   }

   // Calculate diffuse lighting
   vec3 norm = normalize(Normal);
   vec3 lightDir = normalize(light.position - FragPos);
   float diff = max(dot(norm, lightDir), 0.0);
   vec3 diffuse = vec3(1.0);
   if(hasDiffuse) {
      vec4 textureDiffuse = texture(materialMaps.diffuseMap, TexCoord);
      if(textureDiffuse.a < 0.5) {
         discard;
      }
      diffuse = light.diffuse * diff * vec3(textureDiffuse);
   }else {
      diffuse = light.diffuse * (diff * material.diffuse);
   }
 
   // Calculate specular lighting
   vec3 viewDir = normalize(viewPos - FragPos);
   vec3 reflectDir = reflect(-lightDir, norm);
   float spec = 0.0;
   if(blinn) {
      vec3 halfwayDir = normalize(lightDir + viewDir);
      spec = pow(max(dot(norm, halfwayDir), 0.0), material.shininess);
   } else {
      spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
   }
   vec3 specular = vec3(1.0);
   if(hasSpecular) {
      vec3 specularMapInfo = vec3(texture(materialMaps.specularMap, TexCoord));
      specular = light.specular * spec * specularMapInfo;
   }else {
      specular = light.specular * (spec * material.specular);
   }

   vec3 emission = vec3(0.0);
   if(hasEmission) {
       emission = texture(materialMaps.emissionMap, TexCoord).rgb;
   }

   // Calculate cut off (Spot Light)
   if(isSpotLight) {
      float theta = dot(lightDir, normalize(-light.direction));
      float epsilon = (light.cutOff - light.outerCutOff);
      float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
      diffuse  *= intensity;
      specular *= intensity;
   }

   // Calculate attenuation (Point Light and Spot Light)
   if(isPointLight || isSpotLight) {
       float distance    = length(light.position - FragPos);
       float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

       ambient  *= attenuation;
       diffuse  *= attenuation;
       specular *= attenuation;
   }

   vec3 result = (ambient + diffuse + specular + emission) * light.color;
   return result;
}

void main() {

   vec3 lightColor = vec3(0.0);
   for(int i = 0; i < nLights; i ++) {
      lightColor += getLightColor(lights[i]);
   }

   vec4 color = vec4(lightColor, 1.0);
   FragColor = color;

   if(gammaCorrection) {
      float gamma = 2.2;
      FragColor.rgb = pow(FragColor.rgb, vec3(1.0/gamma));
   }
}