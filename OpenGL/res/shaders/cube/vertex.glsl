#version 330 core

layout(location = 0) in vec3 pos;
//layout(location = 1) in vec2 texcoord;
layout(location = 2) in vec3 color;
layout(location = 3) in vec3 normal;

//out vec2 Ttexcoord;
out vec3 Tcolor;
out vec3 Tnormal;
out vec3 Wpos;
out float clipW;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
	// 在frag中通过调用gl_FragCoord再逆变换计算出顶点的模型坐标，精度高
	//【这种逆变换需要一个clipW（裁剪空间的w），计算开销较大】
	mat4 transform = projection * view * model;
	vec4 clipPos = transform * vec4(pos, 1.0f);
	gl_Position = clipPos;
	clipW = clipPos.w;
	// 直接传Wpos（顶点的世界坐标）给frag，通过插值获得，精度较低
	//【这种插值易受顶点情况与数量影响，在透视和非线性变换后不准确】
	Tcolor = color;
	Tnormal = mat3(transpose(inverse(model))) * normal;
	Wpos = vec3(model * vec4(pos, 1.0f));
	//Ttexcoord = vec2(texcoord.x, 1.0 - texcoord.y);
}