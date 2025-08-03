#version 330 core

in vec3 Tcolor;

out vec4 color;

void main() {
  color = vec4(Tcolor, 1.0f);
}