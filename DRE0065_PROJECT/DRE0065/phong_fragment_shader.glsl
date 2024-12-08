#version 400
in vec3 fragPos;
in vec3 normal;

out vec4 out_Color;

struct Material 
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

uniform Material material;
uniform vec3 lightPosition;
uniform vec3 viewPos;
uniform vec3 lightColor;

void main(void) 
{
    vec3 ambient = material.ambient * lightColor;

    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(lightPosition - fragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = material.diffuse * diff * lightColor;

    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = material.specular * spec * lightColor;

    vec3 result = ambient + diffuse + specular;
    out_Color = vec4(result, 1.0);
}