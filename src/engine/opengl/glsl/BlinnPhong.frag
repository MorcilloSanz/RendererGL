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
in vec4 FragPosLightSpace;

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

uniform sampler2D shadowMap;

vec4 calculateAmbient(Light light) {
   vec4 ambient = vec4(1.0);

   vec4 lightAmbient = vec4(light.ambient, 1.0);
   vec4 lightColor = vec4(light.color, 1.0);

   if(hasDiffuse) {
      vec4 textureDiffuse = texture(materialMaps.diffuseMap, TexCoord);
      ambient = lightAmbient * lightColor * textureDiffuse;
   }
   else ambient = lightAmbient * lightColor * vec4(ourColor, 1.0);

   return ambient;
}

vec4 calculateDiffuse(Light light, vec3 normal, vec3 lightDir) {
   vec4 diffuse = vec4(1.0);
   float diff = max(dot(normal, lightDir), 0.0);

   vec4 lightDiffuse = vec4(light.diffuse, 1.0);
   vec4 materialDiffuse = vec4(material.diffuse, 1.0);

   if(hasDiffuse) {
      vec4 textureDiffuse = texture(materialMaps.diffuseMap, TexCoord);
      diffuse = lightDiffuse * diff * textureDiffuse;
   }else diffuse = lightDiffuse * (diff * materialDiffuse);

   return diffuse;
}

vec4 calculateSpecular(Light light, vec3 normal, vec3 lightDir) {
   vec4 specular = vec4(1.0);
   vec3 viewDir = normalize(viewPos - FragPos);
   vec3 reflectDir = reflect(-lightDir, normal);
   float spec = 0.0;

   vec4 lightSpecular = vec4(light.specular, 1.0);
   vec4 materialSpecular = vec4(material.specular, 1.0);

   if(blinn) {
      vec3 halfwayDir = normalize(lightDir + viewDir);
      spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);
   } else spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

   if(hasSpecular) {
      vec4 specularMapInfo = texture(materialMaps.specularMap, TexCoord);
      specular = lightSpecular * spec * specularMapInfo;
   }else specular = lightSpecular * (spec * materialSpecular);

   return specular;
}

vec4 calculateEmission() {
   vec4 emission = vec4(0.0);
   if(hasEmission) emission = texture(materialMaps.emissionMap, TexCoord);
   return emission;
}

float calculateShadow(vec4 fragPosLightSpace) {
   // perform perspective divide
   vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
   // transform to [0,1] range
   projCoords = projCoords * 0.5 + 0.5;
   // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
   float closestDepth = texture(shadowMap, projCoords.xy).r; 
   // get depth of current fragment from light's perspective
   float currentDepth = projCoords.z;
   // calculate bias (based on depth map resolution and slope)
   vec3 normal = normalize(Normal);
   vec3 lightDir = normalize(lightPos - FragPos);
   float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);
   // check whether current frag pos is in shadow
   // float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;
   // PCF
   float shadow = 0.0;
   vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
   for(int x = -1; x <= 1; ++x)
   {
      for(int y = -1; y <= 1; ++y)
      {
         float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
         shadow += currentDepth - bias > pcfDepth  ? 1.0 : 0.0;        
      }    
   }
   shadow /= 9.0;
   
   // keep the shadow at 0.0 when outside the far_plane region of the light's frustum.
   if(projCoords.z > 1.0)
      shadow = 0.0;
      
   return shadow;
}

vec4 getLightColor(Light light) {

   // Calculate ambient, diffuse and specular
   vec3 norm = normalize(Normal);
   vec3 lightDir = normalize(light.position - FragPos);

   vec4 ambient = calculateAmbient(light);
   vec4 diffuse = calculateDiffuse(light, norm, lightDir);
   vec4 specular = calculateSpecular(light, norm, lightDir);

   // Calculate emission
   vec4 emission = calculateEmission();

   // Calculate attenuation (Point Light and Spot Light)
   if(isPointLight || isSpotLight) {
       float distance = length(light.position - FragPos);
       float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
       ambient  *= attenuation;
       diffuse  *= attenuation;
       specular *= attenuation;
   }

   // Calculate cut off (Spot Light)
   if(isSpotLight) {
      float theta = dot(lightDir, normalize(-light.direction));
      float epsilon = (light.cutOff - light.outerCutOff);
      float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
      diffuse  *= intensity;
      specular *= intensity;
   }

   // Calculate shadow
   float shadow = calculateShadow(FragPosLightSpace);

   vec4 lightColor = vec4(light.color, 1.0);
   return (ambient + (1.0 - shadow) * (diffuse + specular) + emission) * lightColor;
}

void main() {

   vec4 lightColor = vec4(0.0);
   for(int i = 0; i < nLights; i ++) lightColor += getLightColor(lights[i]);

   if(gammaCorrection) {
      float gamma = 2.2;
      lightColor.rgb = pow(lightColor.rgb, vec3(1.0 / gamma));
   }

   // Apply transparency for blending
   if(hasDiffuse) {
      vec4 textureDiffuse = texture(materialMaps.diffuseMap, TexCoord);
      lightColor.a = textureDiffuse.a;
   }

   FragColor = lightColor;
}