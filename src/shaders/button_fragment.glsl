#version 450 core

in vec2 rect;
out vec4 ButtonColor;

uniform vec4 color;

void main() {
    ButtonColor = vec4(color);
}