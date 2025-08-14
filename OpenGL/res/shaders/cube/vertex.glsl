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
	// ��frag��ͨ������gl_FragCoord����任����������ģ�����꣬���ȸ�
	//��������任��Ҫһ��clipW���ü��ռ��w�������㿪���ϴ�
	mat4 transform = projection * view * model;
	vec4 clipPos = transform * vec4(pos, 1.0f);
	gl_Position = clipPos;
	clipW = clipPos.w;
	// ֱ�Ӵ�Wpos��������������꣩��frag��ͨ����ֵ��ã����Ƚϵ�
	//�����ֲ�ֵ���ܶ������������Ӱ�죬��͸�Ӻͷ����Ա任��׼ȷ��
	Tcolor = color;
	Tnormal = mat3(transpose(inverse(model))) * normal;
	Wpos = vec3(model * vec4(pos, 1.0f));
	//Ttexcoord = vec2(texcoord.x, 1.0 - texcoord.y);
}