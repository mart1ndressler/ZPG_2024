#version 400
out vec4 out_Color;

uniform vec3 objectColor;

void main(void) {out_Color = vec4(objectColor, 1.0);}