#version 330 core
out vec4 FragColor;

uniform vec3 objectColor = vec3(0.0f, 1.0f, 0.0f);

void main() {FragColor = vec4(objectColor, 1.0f);}