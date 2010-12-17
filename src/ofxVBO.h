#ifndef _OFX_VBO
#define _OFX_VBO


#include "ofMain.h"
#include "ofxShader.h"
#include "ofxVectorMath.h"
#include "ofxFBOTexture.h"
#define NUM_PARTICLES 1000


// ----------------------- Classes for particles
class Pvert {
public:
	float x,y,z;
	Pvert() {
		x=0.0;y=0.0;z=0.0;
	}
};
class Pcolor {
public:
	float r,g,b,a;
	Pcolor() {
		r=0.0;g=0.0;b=0.0;a=0.0;
	}
};


// -----------------------
class ofxVBO {

public:
	void setup();
	void update();
	void draw();

	// Camera
	ofxVec3f tran, tranv;
	ofxVec3f rot, rotv;

	// reset the particle
	void resetParticle(int i);


	// Color and Vertex VBO
	GLuint	 vbo[2];

	float   boxSize;

	Pvert	vel[NUM_PARTICLES];
	Pvert	pnts[NUM_PARTICLES];
	Pcolor	color[NUM_PARTICLES];

	float pointSizes[NUM_PARTICLES];

	ofTexture		texture;
	ofxShader		shader;
	ofxFBOTexture fbo;
	bool bDepthTest;

};

#endif
