#version 330 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 color;
layout(location = 2) in vec2 texcoord;

out vec3 Tcolor;
out vec2 Ttexcoord;

void main() {
  gl_Position = vec4(pos, 1.0f);
  Tcolor = color;
	Ttexcoord = texcoord;
}