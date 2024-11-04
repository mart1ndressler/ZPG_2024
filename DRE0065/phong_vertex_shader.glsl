#version 400
layout (location = 0) in vec3 in_Position;
layout (location = 1) in vec3 in_Normal;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform mat3 normalMatrix;

out vec3 fragPos;
out vec3 normal;

void main(void) 
{
    fragPos = vec3(modelMatrix * vec4(in_Position, 1.0));
    normal = normalize(mat3(transpose(inverse(modelMatrix))) * in_Normal); 
    gl_Position = projectionMatrix * viewMatrix * vec4(fragPos, 1.0);
}