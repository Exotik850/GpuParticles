#version 330

uniform sampler2D position;
uniform sampler2D velocity;
uniform float dt;

in vec2 vertTexCoord;
out vec4 v_color;

void main() {
    vec2 pos = texture(position, vertTexCoord).xy;
    vec2 vel = texture(velocity, vertTexCoord).xy;
    pos += vel * dt;
    
    v_color = vec4(pos, 0.0, 0.0);

}