

#define OF_ADDON_USING_OFXOPENCV
#define OF_ADDON_USING_OFXOBJLOADER
#define OF_ADDON_USING_OFXDIRLIST
#define OF_ADDON_USING_OFXVECTORMATH
#define OF_ADDON_USING_OFXNETWORK
#define OF_ADDON_USING_OFXVECTORGRAPHICS
#define OF_ADDON_USING_OFXOSC
#define OF_ADDON_USING_OFXTHREAD
#define OF_ADDON_USING_OFXXMLSETTINGS

#include "ofMain.h"
#include "ofAddons.h"
#include "ModelHandler.h"
#include "global.h"


class ARHelper
{
public:
    ARHelper();
    bool setup();
    void update(unsigned char *pixels ,int _width ,int _height);
    void draw();
    void videoSettings();
    void toggleMirror();
    void toggleDebug();
    void debugDraw(int id , float *m);
    bool isDetected()
    {
        return markerId>-1?true:false;
    }
    int  getMarkerId()
    {
        return markerId;
    }

    void setModelHandler(ModelHandler &mh)
    {
        modelHandler = &mh;
    }
    void setModelHandler(ModelHandler *mh)
    {
        modelHandler = mh;
    }
    ModelHandler *modelHandler;

protected:
private:
    ofTrueTypeFont myFont;
    //ofVideoGrabber grabber;
    ofxCvColorImage convert;
    ofxCvGrayscaleImage gray;

    int width, height, bpp,markerId,displayW,displayH;
    float conf;
    size_t        numPixels;
    size_t        numBytesRead;
    unsigned char *cameraBuffer;

    bool bDraw;
    bool useBCH;
    bool useCustom;
    bool bMirror;
    bool bDebug;


    ofxVec2f markerV;
    int tol;


    ofImage north;



};

