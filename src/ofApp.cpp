#include "ofApp.h"

#define TEX_WIDTH 1024
#define TEX_HEIGHT 768

//--------------------------------------------------------------
void ofApp::setup(){
    // position = PingPongFbo(TEX_WIDTH, TEX_HEIGHT, 0, ofGetWidth(), 0, ofGetHeight() true);
    // velocity = PingPongFbo(TEX_WIDTH, TEX_HEIGHT, -1, 1, -1, 1, true);
    // velocity.swap();
    // drawShader.load("drawShader");
    // updateShader.load("updateShader");
    ofEnableSmoothing();
    glPointSize(2);
    mouse = {};
    ps = ParticleSystem(1000, TEX_WIDTH, TEX_HEIGHT);
}

//--------------------------------------------------------------
void ofApp::update(){
    ofBackground(0);
    float dt = ofGetLastFrameTime();
    ps.update(dt, mouse);
    // position.begin();
    // position.getFbo().activateAllDrawBuffers();
    // updateShader.begin();
    // updateShader.setUniformTexture("position", position.getTexture(), 0);
    // updateShader.setUniformTexture("velocity", velocity.getTexture(), 1);
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    ps.draw();
}

void ofApp::mouseMoved(int x, int y)
{
    mouse = {static_cast<float>(x), static_cast<float>(y)};
}
