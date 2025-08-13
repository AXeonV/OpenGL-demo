#version 330 core

in vec2 Ttexcoord;
in vec3 Tcolor;
in vec3 Tnormal;
in vec3 VertexPos;
in float clipW;

uniform vec2 u_resolution;
uniform float u_time;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform sampler2D Utexture1;
uniform sampler2D Utexture2;

struct Material {
	float ambient;
	float diffuse;
	float specular;
	float shininess;
};
uniform Material material;

struct Dir_Light {
	vec3 direction;
	vec3 color;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};
uniform Dir_Light dlight;

struct Point_Light {
	vec3 position;
	vec3 color;

	float ambient;
	float diffuse;
	float specular;

	vec3 K;
};
#define NR_POINT_LIGHTS 3
uniform Point_Light plight[NR_POINT_LIGHTS];

struct Camera {
	vec3 position;
};
uniform Camera camera;

const float EPS = 1e-3;

vec3 palette2(float t, float factor) {
	vec3 a = vec3(0.5) + 0.3 * sin(vec3(0.1, 0.3, 0.5) * factor);
	vec3 b = vec3(0.5) + 0.3 * cos(vec3(0.2, 0.4, 0.6) * factor);
	vec3 c = vec3(1.0) + 0.5 * sin(vec3(0.3, 0.7, 0.9) * factor);
	vec3 d = vec3(0.25, 0.4, 0.55) + 0.2 * cos(vec3(0.5, 0.6, 0.7) * factor);
	return a + b * cos(6.28318 * (c * t + d));
}
float hash(vec2 p) {
	return fract(sin(dot(p, vec2(127.1, 311.7))) * 43758.5453123);
}

float noise(vec2 p) {
	vec2 i = floor(p);
	vec2 f = fract(p);
	vec2 u = f * f * (3.0 - 2.0 * f);

	return mix(mix(hash(i + vec2(0.0, 0.0)), hash(i + vec2(1.0, 0.0)), u.x),
		mix(hash(i + vec2(0.0, 1.0)), hash(i + vec2(1.0, 1.0)), u.x),
		u.y);
}

vec2 get_pos() {
	vec3 ndc;
	ndc.x = (gl_FragCoord.x / u_resolution.x) * 2.0 - 1.0;
	ndc.y = (gl_FragCoord.y / u_resolution.y) * 2.0 - 1.0;
	ndc.z = gl_FragCoord.z * 2.0 - 1.0;
	mat4 transform = projection * view * model;
	vec4 clipPos = vec4(ndc.xyz * clipW, clipW);
	vec4 modelPos = inverse(transform) * clipPos;
	modelPos *= 2.0f;
	if (1.0f - abs(modelPos.x) < EPS) return vec2(modelPos.y, modelPos.z);
	if (1.0f - abs(modelPos.y) < EPS) return vec2(modelPos.x, modelPos.z);
	if (1.0f - abs(modelPos.z) < EPS) return vec2(modelPos.x, modelPos.y);
}

vec3 CalcDirLight(Dir_Light light) {
	vec3 ambient = light.ambient * light.color * material.ambient;

	vec3 normal = normalize(Tnormal);
	vec3 lightDir = normalize(-light.direction);
	float diff = max(dot(normal, lightDir), 0.0);
	vec3 diffuse = light.diffuse * light.color * (diff * material.diffuse);

	vec3 viewDir = normalize(camera.position - VertexPos);
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = light.specular * light.color * (spec * material.specular);

	return (ambient + diffuse + specular);
}
vec3 CalcPointLight(Point_Light light) {
	vec3 ambient = light.ambient * light.color * material.ambient;

	vec3 normal = normalize(Tnormal);
	vec3 lightDir = normalize(light.position - VertexPos);
	float diff = max(dot(normal, lightDir), 0.0);
	vec3 diffuse = light.diffuse * light.color * (diff * material.diffuse);

	vec3 viewDir = normalize(camera.position - VertexPos);
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = light.specular * light.color * (spec * material.specular);

	float distance = length(light.position - VertexPos);
	float attenuation = 1.0 / (light.K.x + light.K.y * distance + light.K.z * (distance * distance));
	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	return (ambient + diffuse + specular);
}

void main() {
	vec3 Rcolor = vec3(0.0);
	vec2 FragPos = get_pos();
	vec2 uv = vec2(abs(FragPos.x), FragPos.y);
	float breath = 1.0 + 2.0 * sin(u_time * 0.2) * smoothstep(0.2, 1.5, length(uv));
	uv *= breath;
	uv *= 20.0;

	for (int i = 0; i < 30; i++) {
		float t = u_time * 0.01 - float(i);
		uv *= mat2(cos(t), sin(t), -sin(t), cos(t));
		uv += noise(sin(uv) * 0.6);
		uv += noise(-cos(uv) * 0.6);

		Rcolor += 0.002 / length(uv + sin(t));

		float intensity = 0.1 / length(uv - (10.3) * sin(t)) * (length(uv) * sin(float(i) + u_time));

		Rcolor += palette2(float(i) / u_time, u_time * 0.5) * intensity;
	}

	vec3 Tret = vec3(0.0f);
	for (int i = 0; i < NR_POINT_LIGHTS; ++i)
		Tret += CalcPointLight(plight[i]) * Tcolor;

	gl_FragColor = vec4(Rcolor + 0.5f * Tret, 1.0f);
}