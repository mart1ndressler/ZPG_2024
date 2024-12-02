#version 330 core

in vec3 FragPos;
in vec3 Normal;

out vec4 FragColor;

uniform vec3 viewPos;
uniform int numLights;
uniform vec3 lightPositions[10];
uniform vec3 lightColors[10];

struct Material 
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};
uniform Material material;

void main()
{
    vec3 ambient = material.ambient;
    vec3 diffuse = vec3(0.0);
    vec3 specular = vec3(0.0);
    vec3 norm = normalize(Normal);

    for(int i=0; i < numLights; i++) 
    {
        vec3 lightPos = lightPositions[i];
        vec3 lightColor = lightColors[i];

        vec3 lightDir = normalize(lightPos - FragPos);
        float distance = length(lightPos - FragPos);
        float attenuation = 1.0 / (0.08 + 0.08 * distance + 0.1 * (distance * distance));

        float diff = max(dot(norm, lightDir), 0.0);
        diffuse += diff * lightColor * material.diffuse * attenuation;

        vec3 viewDir = normalize(viewPos - FragPos);
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
        specular += spec * lightColor * material.specular * attenuation;
    }

    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}