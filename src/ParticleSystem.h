#pragma once
#include "ofMain.h"

class ParticleSystem {
public:
    ParticleSystem() {}
    ParticleSystem(int numParticles, int width, int height) :
        m_numParticles(numParticles), m_width(width), m_height(height) {
        // Allocate buffers for particle positions and velocities
        vector<Particle> particles;
        for (int i = 0; i < numParticles; i++) {
            Particle particle;
            float x = ofRandom(width);
            float y = ofRandom(height);
            particle.position = ofVec2f(x, y);
            particle.velocity = particle.position.getNormalized().rotate(90) * ofRandomf() * 5.0f + 0.1f;
            particles.emplace_back(particle);
        }

        // Upload particle data to the GPU
        m_particleBuffer.allocate(particles, GL_DYNAMIC_DRAW);

        // Load compute shader
        m_computeShader.setupShaderFromFile(GL_COMPUTE_SHADER, "particleUpdate.glsl");
        m_computeShader.linkProgram();
    }

    void update(float dt, ofVec2f mouse) {
        // Dispatch compute shader to update particles
        m_particleBuffer.bindBase(GL_SHADER_STORAGE_BUFFER, 0);
        m_computeShader.begin();
        m_computeShader.setUniform1i("numParticles", m_numParticles);
        m_computeShader.setUniform2f("dimensions", m_width, m_height);
        m_computeShader.setUniform2f("mouse", mouse);
        m_computeShader.setUniform1f("gravity", 98.0f);
        m_computeShader.setUniform1f("deltaTime", dt);
        m_computeShader.setUniform1f("noiseScale", 0.1f);
        m_computeShader.setUniform1f("noiseSpeed", 0.01f);
        m_computeShader.dispatchCompute(m_numParticles / 1024 + 1, 1, 1);
        m_computeShader.end();
        m_particleBuffer.unbindBase(GL_SHADER_STORAGE_BUFFER, 0);

    }

    void ofDrawVertexBuffer(const ofBufferObject& buffer, GLenum primitiveMode, GLintptr offset, GLsizei count) {
        // Bind the vertex buffer
        glBindBuffer(GL_ARRAY_BUFFER, buffer.getId());
        // Enable the vertex attribute array
        glEnableVertexAttribArray(0);
        // Set the vertex attribute pointer
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Particle), reinterpret_cast<void*>(offsetof(Particle, position)));        // Draw the vertices
        glDrawArrays(primitiveMode, 0, count);
        // Disable the vertex attribute array
        glDisableVertexAttribArray(0);
        // Unbind the vertex buffer
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void draw() {
        // Draw particles to the screen
        ofSetColor(255, 25);
        ofDrawVertexBuffer(m_particleBuffer, GL_POINTS, 0, m_numParticles);
    }

    

private:
    int m_numParticles;
    int m_width;
    int m_height;

    struct Particle {
        ofVec2f position;
        ofVec2f velocity;
        ofFloatColor color;

        Particle() :
            position(ofVec2f(0, 0)),
            velocity(ofVec2f(0, 0)),
            color(ofFloatColor(ofRandom(1.0f), ofRandom(1.0f), ofRandom(1.0f), 1.0f)) {}
    };

    ofShader m_computeShader;
    ofBufferObject m_particleBuffer;
};