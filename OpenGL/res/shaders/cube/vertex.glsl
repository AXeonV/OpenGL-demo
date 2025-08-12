#version 330 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 texcoord;
layout(location = 2) in vec3 color;
layout(location = 3) in vec3 normal;

out vec2 Ttexcoord;
out vec3 Tcolor;
out vec3 Tnormal;
out vec3 Fpos;
out float clipW;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
	mat4 transform = projection * view * model;
	vec4 clipPos = transform * vec4(pos, 1.0f);
	gl_Position = clipPos;
	Ttexcoord = vec2(texcoord.x, 1.0 - texcoord.y);
	Tcolor = color;
	//Tcolor = vec3(0.2, 0.3, 0.4);
	Tnormal = mat3(transpose(inverse(model))) * normal;
	Fpos = vec3(model * vec4(pos, 1.0f));
	clipW = clipPos.w;
}