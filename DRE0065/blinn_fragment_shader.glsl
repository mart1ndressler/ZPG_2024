#version 400
in vec3 fragPos;
in vec3 normal;

out vec4 out_Color;

uniform vec3 lightPosition;
uniform vec3 viewPos;
uniform vec3 lightColor;

struct Material 
{
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

uniform Material material;

void main(void) 
{
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(lightPosition - fragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor * material.diffuse;

    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(norm, halfwayDir), 0.0), material.shininess);
    vec3 specular = lightColor * material.specular * spec;

    vec3 result = diffuse + specular;
    out_Color = vec4(result, 1.0);
}