#version 330 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 texcoord;
layout(location = 2) in vec3 color;

out vec2 Ttexcoord;
out vec3 Tcolor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
  gl_Position = projection * view * model * vec4(pos, 1.0f);
	Ttexcoord = vec2(texcoord.x, 1.0 - texcoord.y);
	Tcolor = color;
}