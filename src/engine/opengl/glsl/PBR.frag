#version 330 core

#define MAX_LIGHTS 64
#define MAX_TEXTURES 64

struct Material {
    vec3 albedo;
    float metallic;
    float roughness;
    float ao;
    vec3 emission;
}; 

struct Light {
    vec3 position;
    vec3 color;

    float constant;
    float linear;
    float quadratic;

    bool pointLight;
};

struct MaterialMaps {
   sampler2D albedo;
   sampler2D metallic;
   sampler2D roughness;
   sampler2D normalMap;
   sampler2D ao;
   sampler2D emission;
};

in vec3 ourColor;
in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoord;
in vec4 FragPosLightSpace;
in vec3 TangentLightPos;
in vec3 TangentViewPos;
in vec3 TangentFragPos;

out vec4 FragColor;

uniform Light lights[MAX_LIGHTS];
uniform int nLights;

uniform Material material;

uniform MaterialMaps materialMaps;
uniform bool hasAlbedo;
uniform bool hasMetallic;
uniform bool hasRoughness;
uniform bool hasNormalMap;
uniform bool hasAmbientOcclusion;
uniform bool hasEmission;

uniform vec3 viewPos;

const float PI = 3.14159265359;

float distributionGGX(vec3 N, vec3 H, float roughness) {

    float a = roughness * roughness;
    float a2 = a * a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;

    float nom   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return nom / denom;
}

float geometrySchlickGGX(float NdotV, float roughness) {

    float r = (roughness + 1.0);
    float k = (r * r) / 8.0;

    float nom   = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return nom / denom;
}

float geometrySmith(vec3 N, vec3 V, vec3 L, float roughness) {

    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = geometrySchlickGGX(NdotV, roughness);
    float ggx1 = geometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}

vec3 fresnelSchlick(float cosTheta, vec3 F0) {
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

vec3 getNormalFromMap() {

    vec3 tangentNormal = texture(materialMaps.normalMap, TexCoord).xyz * 2.0 - 1.0;

    vec3 Q1  = dFdx(FragPos);
    vec3 Q2  = dFdy(FragPos);
    vec2 st1 = dFdx(TexCoord);
    vec2 st2 = dFdy(TexCoord);

    vec3 N   = normalize(Normal);
    vec3 T  = normalize(Q1 * st2.t - Q2 * st1.t);
    vec3 B  = -normalize(cross(N, T));
    mat3 TBN = mat3(T, B, N);

    return normalize(TBN * tangentNormal);
}

vec3 calculateAlbedo() {
    vec3 albedo = material.albedo;
    if(hasAlbedo) albedo = pow(texture(materialMaps.albedo, TexCoord).rgb, vec3(2.2));
    albedo *= ourColor;
    return albedo;
}

float calculateMetallic() {
    float metallic = material.metallic;
    if(hasMetallic) metallic = texture(materialMaps.metallic, TexCoord).r;
    return metallic;
}

float calculateRoughness() {
    float roughness = material.roughness;
    if(hasRoughness) roughness = texture(materialMaps.roughness, TexCoord).r;
    return roughness;
}

float calculateAmbientOcclusion() {
    float ao = material.ao;
    if(hasAmbientOcclusion) ao = texture(materialMaps.ao, TexCoord).r;
    return ao;
}

vec3 calculateNormal() {
    vec3 N = normalize(Normal);
    if(hasNormalMap) N = getNormalFromMap();
    return N;
}

vec3 calculateEmission() {
    vec3 emission = material.emission;
    if(hasEmission) emission = vec3(texture(materialMaps.emission, TexCoord));
    return emission;
}

void main() {

    // Material
    vec3 albedo = calculateAlbedo();
    float metallic = calculateMetallic();
    float roughness = calculateRoughness();
    float ao = calculateAmbientOcclusion();
    vec3 N = calculateNormal();
    vec3 emission = calculateEmission();

    // V
    vec3 V = normalize(viewPos - FragPos);

    // calculate reflectance at normal incidence; if dia-electric (like plastic) use F0 
    // of 0.04 and if it's a metal, use the albedo color as F0 (metallic workflow)    
    vec3 F0 = vec3(0.04); 
    F0 = mix(F0, albedo, metallic);

    // reflectance equation
    vec3 Lo = vec3(0.0);
    for(int i = 0; i < nLights; i ++)  {

        // calculate per-light radiance
        vec3 L = normalize(lights[i].position - FragPos);
        vec3 H = normalize(V + L);
        
        vec3 radiance = lights[i].color;

        if(lights[i].pointLight) {
            float distance = length(lights[i].position - FragPos);
            float attenuation = 1.0 / (lights[i].constant + lights[i].linear * distance + lights[i].quadratic * (distance * distance));
            radiance *= attenuation;
        }

        // Cook-Torrance BRDF
        float NDF = distributionGGX(N, H, roughness);   
        float G   = geometrySmith(N, V, L, roughness);      
        vec3 F    = fresnelSchlick(max(dot(H, V), 0.0), F0);
           
        vec3 numerator    = NDF * G * F; 
        float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001; // + 0.0001 to prevent divide by zero
        vec3 specular = numerator / denominator;
        
        // kS is equal to Fresnel
        vec3 kS = F;
        // for energy conservation, the diffuse and specular light can't
        // be above 1.0 (unless the surface emits light); to preserve this
        // relationship the diffuse component (kD) should equal 1.0 - kS.
        vec3 kD = vec3(1.0) - kS;
        // multiply kD by the inverse metalness such that only non-metals 
        // have diffuse lighting, or a linear blend if partly metal (pure metals
        // have no diffuse light).
        kD *= 1.0 - metallic;	  

        // scale light by NdotL
        float NdotL = max(dot(N, L), 0.0);        

        // add to outgoing radiance Lo
        Lo += (kD * albedo / PI + specular) * radiance * NdotL;  // note that we already multiplied the BRDF by the Fresnel (kS) so we won't multiply by kS again
    }   

    // replace this ambient lighting with environment lighting).
    vec3 ambient = vec3(0.03) * albedo * ao;

    vec3 color = ambient + emission + Lo;

    // HDR tonemapping
    color = color / (color + vec3(1.0));

    // gamma correct
    color = pow(color, vec3(1.0/2.2)); 

    // Apply transparency
    float transparency = 1.0;
    if(hasAlbedo) {
        vec4 textureAlbedo = texture(materialMaps.albedo, TexCoord);
        transparency = textureAlbedo.a;
    }

    // Color
    FragColor = vec4(color, transparency);
}