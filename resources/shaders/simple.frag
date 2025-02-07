#version 330 core

in vec3 vFragPos;       // Fragment position in world space
in vec3 vNormal;        // Normal vector in world space
in vec2 vTexCoord;      // Texture coordinates

out vec4 FragColor;


uniform vec3 uColor;

uniform vec3 uLightPos;      // Light position in world space
uniform vec3 uViewPos;       // Camera position in world space
uniform vec3 uLightColor;    // Light color
uniform vec3 uAmbientColor;  // Ambient color

uniform sampler2D uTexture;  // Diffuse texture

void main()
{
    // Ambient lighting
    vec3 ambient = uAmbientColor;

    // Diffuse lighting
    vec3 norm = normalize(vNormal);
    vec3 lightDir = normalize(uLightPos - vFragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * uLightColor;

    // Specular lighting
    vec3 viewDir = normalize(uViewPos - vFragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
    vec3 specular = spec * uLightColor;

    // Combine lighting components
    vec3 lighting = ambient + diffuse + specular;

    // Apply texture
    vec3 texColor = texture(uTexture, vTexCoord).rgb;

    // Final color
    //FragColor = vec4(lighting * texColor, 1.0);
    FragColor = vec4(uColor, 1.0);
}
