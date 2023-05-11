#pragma once
#include "ofMain.h"
#include <map>


class PingPongFbo {
public:
    PingPongFbo(unsigned width, unsigned height, bool random = false) {
        pongIndex = 0;
        if (!random) {
            for (int i = 0; i < 2; i++) {
                fboPingPong[i].allocate(width, height, GL_RGBA8);
                fboPingPong[i].begin();
                ofClear(0.0);
                fboPingPong[i].end();
            }
        } else
        {
            float tex[width * height * 4];
            for (int i = 0; i < 2; i++) {
                for(int x = 0; x < width; ++x)
                {
                    for(int y = 0; y < height; ++y)
                    {
                        int index = y * width + x;
                        tex[index] = ofRandomf() * 2.0 - 1.0;
                        tex[index + 1] = ofRandomf() * 2.0 - 1.0;
                        tex[index + 2] = 0.0f;
                        tex[index + 3] = 0.0f;
                    }
                }
                
                fboPingPong[i].getTexture().bind();
                glTexSubImage2D(GL_TEXTURE_RECTANGLE, 0, 0, 0, width, height, GL_RGBA, GL_FLOAT, tex);
                fboPingPong[i].getTexture().unbind();
            }
            delete[] tex;
        }
    }

    void begin() {
        fboPingPong[pongIndex].begin();
    }

    void end() {
        fboPingPong[pongIndex].end();
        swap();
    }

    ofFbo& getFbo() {
        return fboPingPong[pongIndex];
    }

private:
    ofFbo fboPingPong[2];
    int pongIndex;

    void swap() {
        pongIndex = 1 - pongIndex;
    }
};

class GpuParticles
{
    ofShader drawShader;
    ofShader updateShader;
    map<string, PingPongFbo> pingPong;
    ofMesh pointMesh;
    unsigned width, height;
    
public:
    GpuParticles() {}
    void init(unsigned width, unsigned height, vector<string> names, string draw_shader, string update_shader);
    void makeFbo(string name);
    void update(float dt);
    void draw();
    
};
