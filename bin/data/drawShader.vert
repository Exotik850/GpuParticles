#version 330

uniform sampler2DRect position; // position

in vec2 texcoord;

out vec4 vertColor;

void main()
{
    vec4 texel0 = texture(position, texcoord);
    vec4 pos = vec4(texel0.xyz, 1);

    gl_Position = pos;

    vec4 color = vec4(1.0, 1.0, 1.0, 0.2);
    vertColor = color;
}