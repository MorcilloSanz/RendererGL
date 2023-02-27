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

vec3 calculateAmbient(Light light) {
   vec3 ambient = vec3(1.0);

   if(hasDiffuse) {
      ambient = light.ambient * light.color * vec3(texture(materialMaps.diffuseMap, TexCoord));
   }else {
      ambient = light.ambient * light.color * ourColor;
   }

   return ambient;
}

vec3 calculateDiffuse(Light light, vec3 normal, vec3 lightDir) {
   vec3 diffuse = vec3(1.0);
   float diff = max(dot(normal, lightDir), 0.0);

   if(hasDiffuse) {
      vec4 textureDiffuse = texture(materialMaps.diffuseMap, TexCoord);
      if(textureDiffuse.a < 0.5) {
         discard;
      }
      diffuse = light.diffuse * diff * vec3(textureDiffuse);
   }else {
      diffuse = light.diffuse * (diff * material.diffuse);
   }

   return diffuse;
}

vec3 calculateSpecular(Light light, vec3 normal, vec3 lightDir) {
   vec3 specular = vec3(1.0);
   vec3 viewDir = normalize(viewPos - FragPos);
   vec3 reflectDir = reflect(-lightDir, normal);
   float spec = 0.0;

   if(blinn) {
      vec3 halfwayDir = normalize(lightDir + viewDir);
      spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);
   } else {
      spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
   }

   if(hasSpecular) {
      vec3 specularMapInfo = vec3(texture(materialMaps.specularMap, TexCoord));
      specular = light.specular * spec * specularMapInfo;
   }else {
      specular = light.specular * (spec * material.specular);
   }

   return specular;
}

vec3 calculateEmission() {
   vec3 emission = vec3(0.0);
   if(hasEmission) {
       emission = texture(materialMaps.emissionMap, TexCoord).rgb;
   }
   return emission;
}

vec3 getLightColor(Light light) {

   // Calculate ambient
   vec3 ambient = calculateAmbient(light);

   // Calculate diffuse and specular lighting
   vec3 norm = normalize(Normal);
   vec3 lightDir = normalize(light.position - FragPos);

   vec3 diffuse = calculateDiffuse(light, norm, lightDir);
   vec3 specular = calculateSpecular(light, norm, lightDir);

   // Calculate emission
   vec3 emission = calculateEmission();

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
      FragColor.rgb = pow(FragColor.rgb, vec3(1.0 / gamma));
   }
}