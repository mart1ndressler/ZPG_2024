#version 400
layout (location = 0) in vec3 in_Position;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

void main(void) {gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(in_Position, 1.0);}