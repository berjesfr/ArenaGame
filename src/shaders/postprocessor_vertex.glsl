#version 450 core

layout (location = 0) in vec4 vertex;


out vec2 TexCoords;

uniform bool shake;
uniform float time;

void main() {
	gl_Position = vec4(vertex.xy, 0.0, 1.0);
	if (shake) {
		float strength = 0.01;
		gl_Position.x += cos(time * 10) * strength;
		gl_Position.y += cos(time * 15) * strength;
	}
	TexCoords = vertex.zw; 
}