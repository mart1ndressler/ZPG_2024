#version 400
in vec3 fragPos;
in vec3 normal;

out vec4 out_Color;

uniform vec3 lightPosition;
uniform vec3 viewPos;
uniform vec3 lightColor;
uniform vec3 objectColor;

void main(void) 
{
    float ambientStrength = 0.6;
    vec3 ambient = ambientStrength * lightColor;

    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(lightPosition - fragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor * 1.5;

    float specularStrength = 0.8;
    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;

    vec3 result = (ambient + diffuse + specular) * objectColor;
    out_Color = vec4(result, 1.0);
}