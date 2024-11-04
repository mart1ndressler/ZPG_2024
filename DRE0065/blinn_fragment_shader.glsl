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
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(lightPosition - fragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor * objectColor;

    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(norm, halfwayDir), 0.0), 32);
    vec3 specular = lightColor * spec;

    vec3 result = diffuse + specular;
    out_Color = vec4(result, 1.0);
}