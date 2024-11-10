#version 330 core

layout(location = 0) in vec3 vp;
layout(location = 1) in vec3 in_Normal;

out vec3 FragPos;
out vec3 Normal;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main()
{
    FragPos = vec3(modelMatrix * vec4(vp, 1.0));
    Normal = mat3(transpose(inverse(modelMatrix))) * in_Normal;
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(vp, 1.0);
}