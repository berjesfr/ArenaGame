#version 450 core

layout (location = 0) in vec4 vertex;

out vec2 fragmentColorInput;

uniform int numberOfRows;
uniform int numberOfColumns;
uniform vec2 offset;

uniform mat4 model;
uniform mat4 projection;

void main() {
	gl_Position = projection * model * vec4(vertex.xy, 0.0, 1.0);
	fragmentColorInput = vec2(vertex.z / numberOfColumns, vertex.w / numberOfRows) + offset;
}