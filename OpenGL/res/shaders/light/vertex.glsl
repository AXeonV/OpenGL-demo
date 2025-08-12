#version 330 core

layout(location = 0) in vec3 pos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
	mat4 transform = projection * view * model;
	gl_Position = transform * vec4(pos, 1.0f);
}