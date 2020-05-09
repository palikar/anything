#version 460 core

out vec4 frag_color;

in vec4 col;

void main() {
    frag_color = col;
}
