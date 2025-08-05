#version 330 core

in vec3 Tcolor;
in vec2 Ttexcoord;

out vec4 color;

uniform sampler2D Utexture1;
uniform sampler2D Utexture2;

void main() {
  color = mix(texture(Utexture1, Ttexcoord), texture(Utexture2, Ttexcoord), 0.8) * vec4(Tcolor, 1.0f);
}