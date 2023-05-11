#include "GpuParticles.h"

void GpuParticles::init(unsigned width, unsigned height, vector<string> names, string draw_shader = "drawShader", string update_shader = "updateShader")
{
    width = width;
    height = height;
    for(string name : names)
    {
        makeFbo(name);
    }
    for(int i = 0; i < width; ++i)
    {
        for(int j = 0; j < height; ++j)
        {
            pointMesh.addVertex(ofVec3f(0.0));
            pointMesh.addTexCoord(ofVec2f(i, j));
        }
    }
    pointMesh.setMode(OF_PRIMITIVE_POINTS);

    drawShader.load(draw_shader);
    updateShader.load(update_shader);
}

void GpuParticles::makeFbo(string name)
{
    pingPong.insert({name, PingPongFbo(width, height, true)});
}

void GpuParticles::update(float dt)
{
}

void GpuParticles::draw()
{
}
