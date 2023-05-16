#version 330 core

// Input color from the vertex shader
in vec4 frag_color;

// Output color to the screen
out vec4 out_color;

void main() {
    // Set the output color
    out_color = frag_color;
}