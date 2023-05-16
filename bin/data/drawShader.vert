#version 330 core

// Vertex attributes (match the vertex attribute pointers in the function)
layout(location = 0) in vec2 in_position;
layout(location = 1) in vec4 in_color;

uniform vec2 dimensons;
// Output color to the fragment shader
out vec4 frag_color;

void main() {
    // Set the position of the vertex
    gl_Position = vec4(in_position * 0.001, 0.0, 1.0);

    // Pass the color to the fragment shader
    frag_color = in_color;
}