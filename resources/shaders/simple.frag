#version 330 core

in vec3 vFragPos;       // Fragment position in world space
in vec3 vNormal;        // Normal vector in world space
in vec2 vTexCoord;      // Texture coordinates


uniform vec3 uColor;
uniform vec3 uLightPos;      // Light position in world space
uniform vec3 uViewPos;       // Camera position in world space
uniform vec3 uLightColor;    // Light color
uniform vec3 uAmbientColor;  // Ambient color
uniform sampler2D uTexture;  // Diffuse texture
uniform bool uHasTexture; // To check if model has texture
uniform float uSpecularStrength;  // Intensity of specular highlights
uniform float uSpecularPower;     // Shininess exponent
uniform vec3 uLightAttenuation;

out vec4 FragColor;

void main()
{
    // Calculate distance-based attenuation
    float distance = length(uLightPos - vFragPos);
    float attenuation = 1.0 / (uLightAttenuation.x +
    uLightAttenuation.y * distance +
    uLightAttenuation.z * (distance * distance));

    // Ambient lighting
    vec3 ambient = uAmbientColor;

    // Diffuse lighting (unchanged from Phong)
    vec3 norm = normalize(vNormal);
    vec3 lightDir = normalize(uLightPos - vFragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * uLightColor * attenuation;

    // Blinn-Phong Specular (modified part)
    vec3 viewDir = normalize(uViewPos - vFragPos);
    vec3 halfVec = normalize(lightDir + viewDir);  // Halfway vector
    float spec = pow(max(dot(norm, halfVec), 0.0), uSpecularPower);
    vec3 specular = uSpecularStrength * spec * uLightColor * attenuation;

    // Combine components
    vec3 lighting = ambient + diffuse + specular;
    // If model doesnt have texture use color
    vec3 baseColor = uHasTexture ? texture(uTexture, vTexCoord).rgb : uColor;

    FragColor = vec4(lighting * baseColor, 1.0);
}