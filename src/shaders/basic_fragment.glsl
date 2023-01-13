#version 450 core

in vec2 fragmentColorInput;
out vec4 fragmentColorOutput;


uniform sampler2D tex;
uniform int flip;
uniform bool hit;
uniform float time;
void main() {
    vec4 temp = texture(tex, fragmentColorInput);
    if (hit) {
        temp = vec4(1.0, 0.0, 0.0, 1.0) * temp;
    }
    fragmentColorOutput = temp;
}