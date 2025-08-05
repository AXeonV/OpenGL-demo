#version 330 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 color;
layout(location = 2) in vec2 texcoord;

out vec3 Tcolor;
out vec2 Ttexcoord;

uniform mat4 transform;

void main() {
  gl_Position = transform * vec4(pos, 1.0f);
  Tcolor = color;
	Ttexcoord = vec2(texcoord.x, 1.0 - texcoord.y);
}