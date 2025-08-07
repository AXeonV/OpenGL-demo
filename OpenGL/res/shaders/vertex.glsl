#version 330 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 texcoord;
layout(location = 2) in vec3 color;

out vec2 Ttexcoord;
out vec3 Tcolor;
out float clipW;

uniform mat4 transform;

void main() {
	vec4 clipPos = transform * vec4(pos, 1.0f);
	gl_Position = clipPos;
	Ttexcoord = vec2(texcoord.x, 1.0 - texcoord.y);
	Tcolor = color;
	clipW = clipPos.w;
}