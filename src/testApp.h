#ifndef _TEST_APP
#define _TEST_APP
//note 20101007
//add svaeJPG in ofxImage
//ofxFOBTextureSaver changed save jpg / png opertation
#include "ofMain.h"
#include "ARHelper.h"

#include "ofx3DModelLoader.h"
#ifdef TARGET_WIN32
#ifdef USE_GSTREAMER
#include "ofVideoPlayerGst.h"
#endif//USE_GSTREAMER
#else if TARGET_OSX
//#include "ofxQTKitVideoTexture.h"
#endif //TARGET_WIN32

#include "ofxVBO.h"

#include "global.h"

#include "ParticleManager.h"


#define MAXPATH 1024
#define DEBUG
#define  INSTRUCTION
#include "CameraHandler.h"
#include "myNetworkHandler.h"
#include "CountDown.h"
class testApp : public ofBaseApp , public ofxThread
{

public:
    void setup();
    void update();
    void draw();

    void keyPressed  (int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void threadedFunction();



    void fadeIn(ofEventArgs &e);
    void arModelActived(ofAREventArgs &e);
    void arModelDeactived(ofAREventArgs &e);
    void networkMessage(OSCEventArgs &e);
    void afterSave(ofEventArgs &e);
    void Shot(ofEventArgs &e);


    void setUpInstructionVideo();
    void clearStorage();
    void sendMail(string addr);

//    bool bWideScreen;
    bool bDebug;


    ARHelper *arhelper;
    ofx3DModelLoader model;
    ofImage loading;
    ofImage intro;
    ofxFadableBase fadable;
    ModelHandler *modelHandler;

#ifdef TARGET_WIN32

#ifdef USE_GSTREAMER
    ofVideoPlayerGst player;
#else
    ofVideoPlayer player;
#endif //USE_GSTREAMER
#else if TARGET_OSX
    ofVideoPlayer player;


#endif //TARGET_WIN32
    ofxTimer timer;

    ofxXmlSettings xml;

    bool bSetup;
    int vw,vh;
    int x,y;
    float tol;
    float timeout;
    float maskA;
    bool bLoaded;
    bool bText;
#ifdef INSTRUCTION
    ofxShader shaderBGSub;

    ofTexture myTexture,myTexture2;

    ofImage secondImage;
#endif
    ofTrueTypeFont myFont;
//------------------------------------------FBO saver;
    ofxFBOTextureSaver fboSaver;
    int fboWidth,fboHeight;
    bool bCapture;

    CameraHandler camera;
    bool useAR;

    myNetworkHandler network;
    bool                bNETWORKMode;
    string				tmpLocalHost;
    int					tmpPort;
    int					tmpFlashPort;
    bool bChangeBG,bChangeFG;
    bool fgLoaded;
    ofImage fgImg;

    //captures
    bool bShow;
    string lastCaptureFile;
    string sendImagesFile;
    ofxImage lastCapImage,tempCap;
    bool canLoadImage;
    int MAX_CAP_FILE;


    bool bFroeGoundPalyer;
    vector <string> movie_list;
    int num_of_fg;
    bool fgChange,bLoop;
    int bKiss;
    int activeId;
    void loadMovie(int id);
    void closeMovie();
    float vidScale;
    ofxFadableBase loadingMask;
    int mx,my;


//particle manager
    ParticleManager pm;
//counter
    CountDown countDown;


};

#endif
