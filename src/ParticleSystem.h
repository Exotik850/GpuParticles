#pragma once
#include "ofMain.h"

class ParticleSystem {
public:
    ParticleSystem() {}
    ParticleSystem(int numParticles, int width, int height) :
        m_numParticles(numParticles),
        m_dimensions(width, height)
    {
        m_drawShader.load("drawShader");
        m_drawShader.linkProgram();
        // Allocate buffers for particle positions and velocities
        vector<Particle> particles(numParticles);
        for (int i = 0; i < numParticles; i++) {
            particles[i].position = ofVec2f(ofRandom(width), ofRandom(height));
            particles[i].velocity = particles[i].position.getNormalized().rotate(90) * ofRandomf() * 5.0f + 0.1f;
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
        m_computeShader.setUniform2f("dimensions", m_dimensions);
        m_computeShader.setUniform2f("mouse", mouse);
        m_computeShader.setUniform1f("gravity", 98.0f);
        m_computeShader.setUniform1f("deltaTime", dt);
        m_computeShader.setUniform1f("time", ofGetElapsedTimef());
        m_computeShader.setUniform1f("noiseScale", 0.1f);
        m_computeShader.setUniform1f("noiseSpeed", 0.01f);
        m_computeShader.dispatchCompute(m_numParticles / 1024 + 1, 1, 1);
        m_computeShader.end();
        m_particleBuffer.unbind(0);
        m_particleBuffer.unbindBase(GL_SHADER_STORAGE_BUFFER, 0);

    }

    void ofDrawVertexBuffer(const ofBufferObject& buffer, GLenum primitiveMode, GLintptr offset, GLsizei count) {
        // Bind the vertex buffer
        buffer.bind(GL_ARRAY_BUFFER);
        m_drawShader.begin();
        m_drawShader.setUniform2f("dimensions", m_dimensions);
        // Enable the vertex attribute arrays
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        // Set the vertex attribute pointers
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Particle), reinterpret_cast<void*>(offsetof(Particle, position)));
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Particle), reinterpret_cast<void*>(offsetof(Particle, color)));
        // Draw the vertices
        glDrawArrays(primitiveMode, offset, count);
        // Disable the vertex attribute arrays
        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
        m_drawShader.end();
        // Unbind the vertex buffer
        buffer.unbind(GL_ARRAY_BUFFER);
    }
    
    void draw() {
        // Draw particles to the screen
        m_particleBuffer.bindBase(GL_SHADER_STORAGE_BUFFER, 0);
        ofDrawVertexBuffer(m_particleBuffer, GL_POINTS, 0, m_numParticles);
        m_particleBuffer.unbindBase(GL_SHADER_STORAGE_BUFFER, 0);
    }

private:
    int m_numParticles;
    ofVec2f m_dimensions;

    struct Particle {
        ofVec2f position;
        ofVec2f velocity;
        ofFloatColor color;

        Particle() :
            position(ofVec2f(0, 0)),
            velocity(ofVec2f(0, 0)),
            color(ofFloatColor(ofRandom(1.0f), ofRandom(1.0f), ofRandom(1.0f), 0.1f)) {}
    };

    ofShader m_computeShader, m_drawShader;
    ofBufferObject m_particleBuffer;
};