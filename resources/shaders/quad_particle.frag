#version 460 core

out vec4 frag_color;

in float col;

void main() {
    frag_color = vec4(1.0, col, 0.0, 1.0);
}
