#pragma once



class PingPongFbo {
public:
    PingPongFbo() {};
    PingPongFbo(int width, int height, float x0, float x1, float y0, float y1, bool random = false) {
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
            float tex[1024 * 768 * 4];
            for (int i = 0; i < 2; i++) {
                for(int x = 0; x < width; ++x)
                {
                    for(int y = 0; y < height; ++y)
                    {
                        int index = y * width + x;
                        tex[index] = map(ofRandomf(), 0, 1, x0, x1);
                        tex[index + 1] = map(ofRandomf(), 0, 1, y0, y1);
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

    void bind()
    {
        fboPingPong[pongIndex].bind();
    }

    void unbind()
    {
        fboPingPong[pongIndex].unbind();
        swap();
    }

    void begin() {
        fboPingPong[pongIndex].begin();
    }

    void end() {
        fboPingPong[pongIndex].end();
        swap();
    }

    ofTexture& getTexture()
    {
        return fboPingPong[1 - pongIndex].getTexture();
    }

    ofFbo& getFbo() {
        return fboPingPong[pongIndex];
    }

    ofFbo& getOtherFbo() {
        return fboPingPong[1 - pongIndex];
    }

    void swap() {
        pongIndex = 1 - pongIndex;
    }

private:
    ofFbo fboPingPong[2];
    int pongIndex;
    
    float map(float x, float in_min, float in_max, float out_min, float out_max)
    {
        return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
    }
};
