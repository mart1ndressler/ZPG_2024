#version 400
in vec3 fragPos;
in vec3 normal;

out vec4 out_Color;

uniform vec3 lightPosition;
uniform vec3 lightColor;

struct Material 
{
    vec3 diffuse;
};

uniform Material material;

void main() 
{
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(lightPosition - fragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor * material.diffuse;

    out_Color = vec4(diffuse, 1.0);
}