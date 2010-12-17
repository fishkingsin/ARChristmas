/*
 *  chromakeyShader.cpp
 *  openFrameworks
 *
 *  Created by theo on 11/06/2010.
 *  Copyright 2010 yU+co.[lab] All rights reserved.
 *
 */

#include "chromakeyShader.h"

//--------------------------------------------------------------
void chromakeyShader::setup(int fboW, int fboH){

	ofBackground(255,255,255);
	ofSetVerticalSync(true);

	_w = fboW;
	_h = fboH;

	//fbo1.allocate(fboW, fboH, true);
	//fbo2.allocate(fboW, fboH, true);

	shaderCK.loadShader("shaders/bg+chroma_keying");

	// PREPARE TEXTURES

	// DisableArbTex !!!
	//////////////////
	//ofDisableArbTex();

	////////////For use as background after chromakey
	myImg.loadImage("images/Squirrel.jpg");
	myImg.setImageType(OF_IMAGE_COLOR);
	myImg.resize(fboW, fboH);

	ofTexture& myTextureRef = myImg.getTextureReference();

	////////////For use as empty blue/green background for subtraction
	myImg2.loadImage("images/Crate.jpg");
	myImg2.setImageType(OF_IMAGE_COLOR);
	myImg2.resize(fboW, fboH);

	ofTexture& myTextureRef2 = myImg2.getTextureReference();

	///// multitextureLoad stuff
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_RECTANGLE_ARB , myTextureRef.getTextureData().textureID);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_RECTANGLE_ARB , myTextureRef2.getTextureData().textureID);

	// go back to default texture unit !!
	glActiveTexture(GL_TEXTURE0);

	// PREPARE CHROMAKEY PARAMETER
	//
	tol = 2.0;
	_r = 0;
	_g = 255;
	_b = 0;
}

//--------------------------------------------------------------
void chromakeyShader::beginRender(){
	//fbo1.swapIn();
}

//--------------------------------------------------------------
void chromakeyShader::endRender(){
	//fbo1.swapOut();
}

//--------------------------------------------------------------
void chromakeyShader::setShaderActive(bool bActive){
	shaderCK.setShaderActive(bActive);

	if(bActive){
		shaderCK.setUniform("tol", tol);
		shaderCK.setUniform("keycolor", _r/255.0f,_g/255.0f,_b/255.0f,1.0);
		shaderCK.setUniform("tex1", 1);
		shaderCK.setUniform("tex2", 2);
	}
}

//--------------------------------------------------------------
void chromakeyShader::setTolParams( float _tol){
	tol	= _tol;
}

//--------------------------------------------------------------
// Set the background image to be seen when the foreground is keyed
void chromakeyShader::setBackgroundImage(ofImage *newImage){
ofLog(OF_LOG_VERBOSE,"setBackgroundImage");
	////////////For use as background after chromakey
	myImg.setFromPixels(newImage->getPixels(), (int)newImage->getWidth(), (int)newImage->getHeight(), OF_IMAGE_COLOR);
	myImg.resize(_w, _h);

	ofTexture& myTextureRef = myImg.getTextureReference();

	///// multitextureLoad stuff
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_RECTANGLE_ARB , myTextureRef.getTextureData().textureID);

	// go back to default texture unit !!
	glActiveTexture(GL_TEXTURE0);

}

//--------------------------------------------------------------
// Set the reference image of an empty stage for "background removal"
void chromakeyShader::setReferenceImage(ofImage *newImage){

	////////////For use as background after chromakey
	myImg2.setFromPixels(newImage->getPixels(), (int)newImage->getWidth(), (int)newImage->getHeight(), OF_IMAGE_COLOR);
	myImg2.resize(_w, _h);

	ofTexture& myTextureRef2 = myImg2.getTextureReference();

	///// multitextureLoad stuff
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_RECTANGLE_ARB , myTextureRef2.getTextureData().textureID);

	// go back to default texture unit !!
	glActiveTexture(GL_TEXTURE0);
}

//--------------------------------------------------------------
// Set the background image to be seen when the foreground is keyed
void chromakeyShader::setBackgroundImage(ofxCvImage *newImage){
ofLog(OF_LOG_VERBOSE,"setBackgroundImage(ofxCvImage *newImage){");
	////////////For use as background after chromakey
	myImg.setFromPixels(newImage->getPixels(), (int)newImage->getWidth(), (int)newImage->getHeight(), OF_IMAGE_COLOR);
	myImg.resize(_w, _h);

	ofTexture& myTextureRef = myImg.getTextureReference();

	///// multitextureLoad stuff
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_RECTANGLE_ARB , myTextureRef.getTextureData().textureID);

	// go back to default texture unit !!
	glActiveTexture(GL_TEXTURE0);
}

//--------------------------------------------------------------
// Set the reference image of an empty stage for "background removal"
void chromakeyShader::setReferenceImage(ofxCvImage *newImage){

	////////////For use as background after chromakey
	myImg2.setFromPixels(newImage->getPixels(), (int)newImage->getWidth(), (int)newImage->getHeight(), OF_IMAGE_COLOR);
	myImg2.resize(_w, _h);

	ofTexture& myTextureRef2 = myImg2.getTextureReference();

	///// multitextureLoad stuff
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_RECTANGLE_ARB , myTextureRef2.getTextureData().textureID);

	// go back to default texture unit !!
	glActiveTexture(GL_TEXTURE0);
}

//--------------------------------------------------------------
void chromakeyShader::setKeyColor(ofColor c){
	_r = c.r;
	_g = c.g;
	_b = c.b;
}

//--------------------------------------------------------------
void chromakeyShader::draw(float x, float y, float w, float h, bool useShader){

	/*
	ofxFBOTexture * src, * dst;
	src = &fbo1;
	dst = &fbo2;

	if( useShader ){
			shaderCK.setShaderActive(true);
			shaderCK.setUniform("tol", tol);
			shaderCK.setUniform("color", _r/255.0f,_g/255.0f,_b/255.0f,1.0);

			dst->swapIn();

			src->draw(0, 0);
			dst->swapOut();

			shaderCK.setShaderActive(false);
	}

	ofEnableAlphaBlending();
	ofSetColor(255, 255, 255, 255);
	dst->draw(x, y, w, h);
	*/

}



