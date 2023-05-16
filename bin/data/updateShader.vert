#version 330

uniform sampler2D position;
uniform sampler2D velocity;
uniform float dt;

in vec4 pos;
in vec2 texcoord;

out vec2 vertTexCoord;

void main() {
    gl_Position = pos;
    vertTexCoord = texcoord;
}