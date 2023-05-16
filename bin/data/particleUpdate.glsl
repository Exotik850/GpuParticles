#version 430

layout(local_size_x = 1024, local_size_y = 1, local_size_z = 1) in;

struct Particle {
    vec2 position;
    vec2 velocity;
    vec4 color;
};

layout(std430, binding = 0) buffer ParticleBuffer {
    Particle particles[];
};

uniform int numParticles;
uniform vec2 dimensions;
uniform float deltaTime;
uniform float time;

const float separationRadius = 25;
const float separationWeight = 4;
const float alignmentRadius = 25;
const float alignmentWeight = 3;
const float cohesionRadius = 100;
const float cohesionWeight = 2;
const float maxSpeed = 150;
const float maxForce = 30;

vec2 limit(vec2 p, float len) {
    float dist = length(p);
    if (dist > len) {
        p /= dist;
        p *= len;
    }
    return p;
}

vec2 getForce(int index) {
    vec2 steering = vec2(0.0);
    vec2 averageAlignment = vec2(0.0);
    vec2 averageCohesion = vec2(0.0);
    int separationCount = 0;
    int alignmentCount = 0;
    int cohesionCount = 0;
    for (int i = 0; i < numParticles; i++) {
        if (i == index) continue;
        vec2 diff = particles[index].position - particles[i].position;
        float dist = length(diff);
        if (dist < separationRadius) {
            diff /= dist;
            steering += diff;
            separationCount++;
        }
        if (dist < alignmentRadius) {
            averageAlignment += particles[i].velocity;
            alignmentCount++;
        }
        if (dist < cohesionRadius) {
            averageCohesion += particles[i].position;
            cohesionCount++;
        }
    }
    if (separationCount > 0) {
        steering /= float(separationCount);
        steering = normalize(steering) * maxSpeed - particles[index].velocity;
    }
    if (alignmentCount > 0) {
        averageAlignment /= float(alignmentCount);
        averageAlignment = normalize(averageAlignment) * maxSpeed - particles[index].velocity;
    }
    if (cohesionCount > 0) {
        averageCohesion /= float(cohesionCount);
        vec2 desired = averageCohesion - particles[index].position;
        desired = normalize(desired) * maxSpeed - particles[index].velocity;
        averageCohesion = desired;
    }
    vec2 totalForce = limit(steering * separationWeight + averageAlignment * alignmentWeight + averageCohesion * cohesionWeight, maxForce);
    return totalForce;
}

void main() {
    int index = int(gl_GlobalInvocationID.x);

    if (index >= numParticles) {
        return;
    }

    Particle particle = particles[index];

    vec2 force = getForce(index);

    particle.velocity += force * deltaTime;
    particle.velocity = limit(particle.velocity, maxSpeed);
    particle.position += particle.velocity * deltaTime;

    // Wrap position around edges of screen
    if (particle.position.x < 0) {
        particle.position.x += dimensions.x;
    } else if (particle.position.x >= dimensions.x) {
        particle.position.x -= dimensions.x;
    }
    if (particle.position.y < 0) {
        particle.position.y += dimensions.y;
    } else if (particle.position.y >= dimensions.y) {
        particle.position.y -= dimensions.y;
    }

    particles[index] = particle;
}