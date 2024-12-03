#version 330 core

in vec3 FragPos;
in vec3 Normal;

out vec4 FragColor;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 lightColor;
uniform vec3 objectColor;

uniform bool lightEnabled;

void main()
{
    vec3 result;

    if (lightEnabled) {
        // 1. Ambient Lighting
        float ambientStrength = 0.1;
        vec3 ambient = ambientStrength * lightColor;

        // 2. Diffuse Lighting
        vec3 norm = normalize(Normal);
        vec3 lightDir = normalize(lightPos - FragPos);
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = diff * lightColor;

        // 3. Specular Lighting
        float specularStrength = 0.5;
        vec3 viewDir = normalize(viewPos - FragPos);
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32); // Shininess = 32
        vec3 specular = specularStrength * spec * lightColor;

        // 4. Combine Results
        result = (ambient + diffuse + specular) * objectColor;
    }
    else {
        result = objectColor;
    }

    FragColor = vec4(result, 1.0);
}