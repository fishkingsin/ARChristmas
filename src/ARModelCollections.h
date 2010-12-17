#pragma once
#include "ARModel.h"
#include "ARImage.h"

#include "global.h"

//#include "ofxNoise.h"
#define startGameTime 5
class ofAREventArgs : public ofEventArgs {
		  public:
			int markerId;
		};
class ARModelCollections
{

public:
    ARModelCollections();
    void setup();
    void update();
    void draw();
    virtual void draw2D();
    void _mouseReleased(ofMouseEventArgs &e);
    //virtual void draw3DTag();
    //virtual void draw2DTag();

    virtual void selfSetup(){printf("ARModelSet selfsetup\n");};
    virtual void selfUpdate(){}

    virtual void fadeIn(){};//use virtual when parent call itself
    virtual void fadeOut(){};//use virtual when parent call itself
    //virtual void drawModel(){}//use virtual when parent call itself
    virtual void drawModel();
    void setModelMatrix(int  _id ,float *_m , float *project);
    virtual void findScreenCoord( const GLdouble *proj, const GLint *view);

    ofEvent <ofAREventArgs> MODEL_ACTIVE;
    ofEvent <ofAREventArgs> MODEL_DEACTIVE;
protected:

    int id;
    vector <ARModel*> models;
    vector <ofImage*> screen_images;
    vector <ARImage*> model_images;

    bool  matrixUpdated;


    float scale;
    float tweenScale;
    float easing;
    float targetEasing;
    float screenX;
    float screenY;
    GLdouble targetMatrix[ 16 ];
    GLdouble m[ 16 ];

    int currentFrame ;
    int lastFrame;
    int exitCount;
    int inactiveCount;
    int timeOut;
    int fadeInCount;
    int fadeOutCount ;
    int numModels;
    int numModelImages;
    int numScreenImages;
    int iseize;

    string model_path;
    string image_path;
    string sound_path;
    string folder;



    ofxFadableBase fadable;

//    ofxEasingLinear 	easinglinear;
//    ofxTween tweenlinear;

    ofxEasingExpo easingexpo;
    ofxTween tweenexpo;
    //ofxPerlin *perlin;

    ofSoundPlayer  sndPlayer;
    float rotX,rotY,rotZ;


};


