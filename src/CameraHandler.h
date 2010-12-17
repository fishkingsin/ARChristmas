#ifndef CAMERAHANDLER_H
#define CAMERAHANDLER_H
#include "CanonCameraWrapper.h"
#include "ofMain.h"
#include "ofxMSAInteractiveObject.h"
#include "ofxOpenCV.h"
#include "ofxSimpleGuiToo.h"
#ifdef USE_CHROMAKEY
#include "chromakeyShader.h"
#endif
#include "ofxImage.h"
#include "ofxFBOTexture.h"
#include "ofVideoPlayerGst.h"
#define USE_LIVEVIDEO
class CameraHandler : public ofxMSAInteractiveObject
{
public:

    virtual ~CameraHandler();
    void setup(int  _width ,int _height);
    void update();
    void draw(int x_, int y_, int w_ ,int h_);
    ofImage *getImage();
    unsigned char* getPixels();
    bool isFrameNew();
    int getWidth();
    int getHeight();

    void mousePressed(int x,int y,int button);
    void chageBackground(string fn);
    void chageBackground(ofImage *img);
    enum
    {
        FILL,
        SKETCH,
        FIT,
    }VIDEO_MODE;
    int mode;
    void setDisplaySize(int width_, int height_);
    void takePhoto();
    void toggleCapBG()
    {
        bCapImage = true;
    }
protected:
    ofColor getColorFromPixels(const unsigned char* pixels,  int w, int h, int type, int x, int y);
private:

    void grabToColorImage(unsigned char *pixels, int width, int height);
    float displayW, displayH ;
    ofxImage *colorImage;
    bool bAllocateImg;
    unsigned char *pixels;

    ofxFBOTexture resizeFBO;
    ofImage bgImage;
    CanonCameraWrapper *canonCamera;		// canon camera control for live view and photo taking
    bool bMirror,hasCanonCamera,hasCamera,bLiveViewMode,hasNewFrame,bKeyingEnabled,bGrabColor,bCapImage;
    ofVideoGrabber *grabber;				// webcam grabber for chromakey
    #ifdef USE_CHROMAKEY
    chromakeyShader keyingShader;
    float tol;
    ofColor grabbedColor;
    #endif
    bool bShooting,bSetBGColor;

    //ofVideoPlayerGst player;
};

#endif // CAMERAHANDLER_H
