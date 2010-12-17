#include "CameraHandler.h"



CameraHandler::~CameraHandler()
{
    if (canonCamera !=NULL)
    {
        ofLog(OF_LOG_VERBOSE,"Camera Handler canonCamera destroy");
        canonCamera->destroy();
        // delete canonCamera ;
    }



    if (grabber!=NULL)
    {
        ofLog(OF_LOG_VERBOSE,"Camera Handler grabber closed");
        //grabber->close();

        //delete grabber;
    }
}

void CameraHandler::setup(int  _width ,int _height)
{
     ofxMSAInteractiveObject::ofxMSAInteractiveObject();
    canonCamera        = NULL;		// canon camera control for live view and photo taking
    grabber            = NULL;
    colorImage          = NULL;
    bAllocateImg = false;
    bLiveViewMode      = true;
    bCapImage          = false;
    bSetBGColor = false;

    disableAllEvents();
    setSize(_width,_height);

    enableMouseEvents();
    resizeFBO.allocate(width,height,true);
    pixels = new unsigned char [int(width*height*3)];
    gui.addPage("Camera");
    gui.addToggle("bLiveViewMode",bLiveViewMode);
    gui.addSlider("VIDEO_MODE",mode,0,2);
    #ifdef USE_CHROMAKEY
    gui.addSlider("shader tol",tol,0.0f,1.0f);
    gui.addToggle("bKeyingEnabled",bKeyingEnabled);
    gui.addToggle("bGrabberColor",bGrabColor);
    gui.addButton("bCapImage",bCapImage);
    gui.addButton("bSetBGColor",bSetBGColor);
    #endif
    gui.addToggle("bMirror",bMirror);
    #ifdef USE_CHROMAKEY
    gui.addSlider("RED",grabbedColor.r,0,255);
    gui.addSlider("GREEN",grabbedColor.g,0,255);
    gui.addSlider("BLUE",grabbedColor.b,0,255);
    #endif
    gui.addToggle("bShooting",bShooting);
    gui.loadFromXML();
    displayW = 640;
    displayH = 480;

    hasCamera = false;
    hasCanonCamera = false;


    /////////////////////////////////////////////////////////////////////
    // CANON CAMERA SETUP
    //

    if (!hasCanonCamera)
    {
        canonCamera = new CanonCameraWrapper();
        hasCamera = hasCanonCamera = canonCamera->setup(0); // Init setup on every start up
        //colorImage = new ofxImage();
        if (hasCanonCamera)  //You must call this to init the canon sdk
        {
            printf("set up success\n");
            canonCamera->setDownloadPath(ofToDataPath("captures"));
            canonCamera->setDeleteFromCameraAfterDownload(true);
            canonCamera->enableDownloadOnTrigger();

            if (canonCamera->openSession())        //Begins communication with camera
            {
                printf("opened session\n");
                //canonCamera.closeSession();

                canonCamera->setDeleteFromCameraAfterDownload(true);
                canonCamera->enableDownloadOnTrigger();
                colorImage = new ofxImage();
                bAllocateImg = false;

//                colorImage->allocate(canonCamera->getLiveViewPixelWidth(),canonCamera->getLiveViewPixelHeight());
            }
        }
        else
        {
            delete canonCamera;
            canonCamera = NULL;
            grabber = new ofVideoGrabber();
            hasCamera = grabber->initGrabber(width, height,true);
            colorImage = new ofxImage();

            colorImage->allocate(width, height,OF_IMAGE_COLOR);
            bAllocateImg = true;
            if (hasCamera)
            {

            }
            else
            {
                delete grabber;
                grabber = NULL;
                colorImage->loadImage("images/test.jpg");
            }
        }

    }
    //player.loadMovie("movies/test.mov");
    #ifdef USE_CHROMAKEY
    keyingShader.setup(width, height);
    keyingShader.setTolParams(tol);

    chageBackground("images/background/bg_number1.jpg");
    #endif
    bCapImage = true;
}
void CameraHandler::update()
{
    if (ofGetFrameNum()==100)
    {
        bCapImage = true;
        bSetBGColor = true;

    }
    #ifdef USE_CHROMAKEY
    keyingShader.setTolParams(tol);
#endif
    if ( hasCamera)
    {
        if (hasCanonCamera && canonCamera)
        {
            // liveView from canon camera automatically
            if (!canonCamera->getLiveViewActive())
            {
                canonCamera->beginLiveView();

            }
            else
            {
                if (canonCamera->getLiveViewFrameNo()>1000000)
                {
                    canonCamera->resetLiveViewFrameCount();
                }
                hasNewFrame = canonCamera->grabPixelsFromLiveView();
                if (hasNewFrame && canonCamera->isLiveViewPixels())
                {
                    //ofLog(OF_LOG_VERBOSE,"Cannon Camera width :%i height: %i",canonCamera->getLiveViewPixelWidth(),canonCamera->getLiveViewPixelHeight());
                    grabToColorImage(canonCamera->getLiveViewPixels(),
                                     canonCamera->getLiveViewPixelWidth(),
                                     canonCamera->getLiveViewPixelHeight());

                }

                resizeFBO.begin();
                colorImage->draw(0,height,width,-height);
                glPixelStorei(GL_PACK_ALIGNMENT, 1);
                glReadPixels(0, 0, resizeFBO.getWidth(), resizeFBO.getHeight(), GL_RGB, GL_UNSIGNED_BYTE, pixels);
                resizeFBO.end();


            }

        }
        else if (grabber)
        {
            // Actively ask for the web camera to update
            grabber->update();
            hasNewFrame = grabber->isFrameNew();
            if (hasNewFrame)
            {
                grabToColorImage(grabber->getPixels(), grabber->getWidth(), grabber->getHeight());

            }
            resizeFBO.begin();
            grabber->draw(0,height,width,-height);
            resizeFBO.end();

        }
        if (bCapImage)
        {
            ofLog(OF_LOG_VERBOSE,"if (bCapImage)");
            //if (hasCanonCamera && canonCamera!=NULL)
            if (colorImage && bAllocateImg)
            {
                ofDisableArbTex();
                ofImage tmp;
                tmp.clone(*colorImage);
                ofEnableArbTex();
                #ifdef USE_CHROMAKEY
                if (bKeyingEnabled)keyingShader.setReferenceImage(&tmp);
                #endif

            }
            /*else if (grabber!=NULL)
            {
                keyingShader.setReferenceImage((ofImage*)grabber);
            }*/
            bCapImage = false;
        }
        if (bSetBGColor)
        {
            #ifdef USE_CHROMAKEY
            if (bKeyingEnabled)keyingShader.setKeyColor(grabbedColor);
            #endif
            bSetBGColor = false;
        }
    }
}
//#define TESTING

void CameraHandler::draw(int x_, int y_, int w_ ,int h_)
{
    if (bLiveViewMode)
    {
#ifdef TESTING
#else
        float _x ,_y,_w,_h = 0;
//    float tcw,tch = 0;

//    if (canonCamera)
//    {
//        tcw = canonCamera->getLiveViewPixelWidth(),
//        tch = canonCamera->getLiveViewPixelHeight();
//
//    }
//    else if (grabber)
//    {
//        tcw = grabber->getWidth();
//        tch = grabber->getHeight();
//    }
        if (mode == FILL)
        {

            _x  = 0;
            _y = 0;
            _w = ofGetWidth();
            _h = ofGetHeight();

        }
        else if (mode == SKETCH)
        {
            if (hasCanonCamera && canonCamera)
            {
                _w = ofGetWidth();
                _h = _w/1.5f;//displayW/3.0f)*2.0f;
                _x  = 0;
                _y = (_h-ofGetHeight())/2.0f;

            }
            else if (grabber )
            {
                _w = ofGetWidth();
                _h = (_w/4.0f)*3.0f;
                _x  = 0;
                _y = (displayH-ofGetHeight())/2.0f;

            }
//            _x  = 0;
//            _y = -(displayH-ofGetHeight())/2;
//            _w = displayW;
//            _h = displayH;
        }
        else if (FIT)
        {
            if (hasCanonCamera && canonCamera)
            {
                _h = ofGetHeight();
                _w = (_h/2.0f)*3.0f;
                _y  = 0;
                _x = -(_h-ofGetWidth())/2.0f;

            }
            else if (grabber )
            {
                _h = ofGetHeight();
                _w = (_h/3.0f)*4.0f;
                _y  = 0;
                _x = -(_w-ofGetWidth())/2.0f;

            }
        }
#endif

#ifdef USE_CHROMAKEY
        // Set shader active
        if (bKeyingEnabled)
            keyingShader.setShaderActive(true);
#endif
        // Draw source image with keying a new background
        if ( hasCamera )
        {
            ofSetColor(255, 255, 255);

            // Setup textures for chromakey
            // TEXTURE 0 : Captured image that going to have the background or color green/blue removed
            // TEXTURE 1 : Background image to be filled after chromakey
            // TEXTURE 2 : A reference image could be used for background subtraction

            // Set Texture 0 active


            // Bind corresponding source for keying

            glPushMatrix();
#ifdef TESTING
            if (bMirror)
            {
                glTranslatef(w_/2,h_/2,0);
                glRotatef(180,0,1,0);
                glTranslatef(-w_/2,-h_/2,0);
            }
            //glDisable(GL_DEPTH_TEST);
            //glEnable(GL_TEXTURE_2D);

            if (hasCanonCamera && canonCamera )resizeFBO.draw(x_,y_+h_,w_,-h_);
            else if (colorImage)colorImage->draw(x_,y_,w_,h_);
#else


            if (bMirror)
            {
                glTranslatef(_w/2,_h/2,0);
                glRotatef(180,0,1,0);
                glTranslatef(-_w/2,-_h/2,0);
            }
            //glDisable(GL_DEPTH_TEST);
            //glEnable(GL_TEXTURE_2D);

            if (hasCanonCamera && canonCamera )resizeFBO.draw(_x,_y+_h,_w,-_h);
            else if (colorImage)colorImage->draw(_x,_y,_w,_h);
#endif
            //glEnable(GL_DEPTH_TEST);
            //glDisable(GL_TEXTURE_2D);

            glPopMatrix();

        }
        #ifdef USE_CHROMAKEY
        // Set shader inactive
        if (bKeyingEnabled)
        keyingShader.setShaderActive(false);
#endif

    }

    if (bGrabColor)
    {
        ofPushStyle();
        ofSetColor(0xffffff);
        if (colorImage && bAllocateImg)
        {
            colorImage->draw(0,0);

            ofFill();
            #ifdef USE_CHROMAKEY
            ofSetColor(grabbedColor.r, grabbedColor.g, grabbedColor.b);
            #endif
            ofRect(colorImage->getWidth(),0,100,100);
            ofSetColor(0xffffff);
            resizeFBO.draw(colorImage->getWidth()+100,0,160,180);
        }
        ofPopStyle();
    }


}
unsigned char* CameraHandler::getPixels()
{
    if (hasCanonCamera && canonCamera && hasNewFrame)
    {
        return pixels;//canonCamera->getLiveViewPixels();

    }
    else if (grabber)
    {

        return grabber->getPixels();
    }
    else return NULL;
//if(colorImage && hasCamera)colorImage->getPixels();
//else return NULL;
}

bool CameraHandler::isFrameNew()
{

    return hasNewFrame;
}
int CameraHandler::getWidth()
{
    if (hasCanonCamera && canonCamera)
    {
        return width;//canonCamera->getLiveViewPixelWidth();

    }
    else if (grabber && !hasCanonCamera)
    {

        return grabber->getWidth();
    }
    else return 0;
//if(colorImage)colorImage->getWidth();
//else return NULL;
//return width;
}

int CameraHandler::getHeight()
{
    if (hasCanonCamera && canonCamera)
    {
        return height;//canonCamera->getLiveViewPixelHeight();

    }
    else if (grabber && !hasCanonCamera)
    {

        return grabber->getHeight();
    }
    else return 0;
//if(colorImage)colorImage->getHeight();
//else return NULL;
//return height;

}

void CameraHandler::mousePressed(int x,int y,int button)
{

    ofLog(OF_LOG_VERBOSE,"CAmera Handler mousepressed");
    #ifdef USE_CHROMAKEY
    if (bGrabColor && colorImage)
    {
        if(x < colorImage->getWidth() && y < colorImage->getHeight())
        grabbedColor = getColorFromPixels(colorImage->getPixels(),
                                          colorImage->width, colorImage->height, OF_IMAGE_COLOR, x, y);

    }
    #endif
}
ofColor CameraHandler::getColorFromPixels(const unsigned char* pixels,  int w, int h, int type, int x, int y)
{
    ofColor color;
    long byteOffset = 0;
    //unsigned char* onePixel = new unsigned char[4];

    int bpp = 0;
    switch (type)
    {
    case OF_IMAGE_GRAYSCALE:
        bpp = 1;
        break;
    case OF_IMAGE_COLOR:
        bpp = 3;
        break;
    case OF_IMAGE_COLOR_ALPHA:
        bpp = 4;
        break;
    }

    byteOffset = (w * bpp) * y + bpp * x;
    //onePixel = (unsigned char *)pixels[(w * bpp) * y + bpp * x];
    if (byteOffset< w*h*bpp)
    {
        switch (type)
        {
        case OF_IMAGE_GRAYSCALE:
            color.r = (float)pixels[byteOffset];
            color.g = (float)pixels[byteOffset];
            color.b = (float)pixels[byteOffset];
            color.a = 255.;
            break;
        case OF_IMAGE_COLOR:
            color.r = (float)pixels[byteOffset];
            color.g = (float)pixels[byteOffset + 1];
            color.b = (float)pixels[byteOffset + 2];
            color.a = 255.;
            break;
        case OF_IMAGE_COLOR_ALPHA:
            color.r = (float)pixels[byteOffset];
            color.g = (float)pixels[byteOffset + 1];
            color.b = (float)pixels[byteOffset + 2];
            color.a = (float)pixels[byteOffset + 3];
            break;
        }

        return color;
    }
    return color;
}
void CameraHandler::setDisplaySize(int width_, int height_)
{
    displayW = 0 ;
    displayH = 0 ;
    // if (hasCanonCamera && canonCamera)
    // {
    //     displayW = width_;
    //     displayH = (width_/3.0f)*2.0f;

    // }
    // else if (grabber && !hasCanonCamera)
    // {

    displayW = width_;
    displayH = (width_/4.0f)*3.0f;


    // }
    ofLog(OF_LOG_VERBOSE,"Camera Handler : set setDisplaySize --- x: " +
          ofToString(displayW)+" y: " + ofToString(displayH));
}
void CameraHandler::chageBackground(string fn)
{
    #ifdef USE_CHROMAKEY
    ofLog(OF_LOG_VERBOSE,"chageBackground "+fn);
    ofDisableArbTex();
    ofImage *img;
    img = new ofImage();
    bool bLoaded = img->loadImage(fn);
    ofEnableArbTex();

    if (bLoaded)
    {

        keyingShader.setBackgroundImage(img);
    }
    img->~ofImage();
#endif

}
void CameraHandler::chageBackground(ofImage *img)
{
    #ifdef USE_CHROMAKEY

    keyingShader.setBackgroundImage(img);
#endif
}
void CameraHandler::grabToColorImage(unsigned char *pixels, int width, int height)
{
    if (colorImage)
    {
        if (!bAllocateImg)
        {
            colorImage->allocate( width, height,OF_IMAGE_COLOR);
            bAllocateImg = true;
        }

        colorImage->setFromPixels(pixels, width,height,OF_IMAGE_COLOR);
    }
}
void CameraHandler::takePhoto()
{
    if (canonCamera && bShooting)
    {
        canonCamera->takePicture();
    }
    else
    {
        ofLog(OF_LOG_WARNING,"Canon Camera is not AVAILIBLE!");
    }
}
