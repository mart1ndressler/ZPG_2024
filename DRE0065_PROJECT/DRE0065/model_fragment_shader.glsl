#version 450

in vec2 uv;
out vec4 Frag_color;

uniform sampler2D tex;

void main () {Frag_color = texture(tex, uv);}