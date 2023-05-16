#pragma once

#include "ofMain.h"
#include "ParticleSystem.h"
// #include "PingPongFbo.h"

class ofApp : public ofBaseApp{

	// PingPongFbo position, velocity;
	// ofShader drawShader, updateShader;
	ParticleSystem ps;
	ofVec2f mouse;
	public:
		void setup();
		void update();
		void draw();
		void mouseMoved(int x, int y) override;
		
};
