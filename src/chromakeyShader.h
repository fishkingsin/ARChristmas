/*
 *  chromakeyShader.h
 *  openFrameworks
 *
 *  Created by theo on 11/06/2010.
 *  Copyright 2010 yU+co.[lab] All rights reserved.
 *
 */

#include "ofMain.h"
#include "ofxShader.h"
#include "ofxCvMain.h"
//#include "ofxFBOTexture.h"

class chromakeyShader{

	public:
		void setup(int fboW, int fboH);

		void beginRender();
		void endRender();

		void setShaderActive(bool bActive);

		void setTolParams(float tol);

		void setBackgroundImage(ofImage *newImage);
		void setReferenceImage(ofImage *newImage);
		void setBackgroundImage(ofxCvImage *newImage);
		void setReferenceImage(ofxCvImage *newImage);

		//void setColor(int r,int g,int b);
		void setKeyColor(ofColor c);
		void draw(float x, float y, float w, float h, bool useShader);

		ofxShader shaderCK;

		// dynamic image as source image buffer

		// loaded image as background after chromakey
        ofImage myImg;

		// loaded image for background subtraction
        ofImage myImg2;

		//ofxFBOTexture fbo1,fbo2;

		int _w, _h;

		float tol;

		int _r,_g,_b;


};
