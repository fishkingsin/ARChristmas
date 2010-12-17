#include "ARHelper.h"
// ----  AR TK+ STUFF - ripped from the multi marker demo app
// needs a home in a nice ofxARToolkit addon :)
#ifdef SINGE_MARKER
#include "ARToolKitPlus/TrackerSingleMarkerImpl.h"
#else
#include "ARToolKitPlus/TrackerMultiMarkerImpl.h"
#endif

class MyLogger : public ARToolKitPlus::Logger
{
    void artLog(const char* nStr)
    {
        printf(nStr);
    }
};

static    MyLogger      logger;
#ifdef SINGE_MARKER
ARToolKitPlus::TrackerSingleMarker *tracker;
#else
ARToolKitPlus::TrackerMultiMarker *tracker;
#endif
ARHelper::ARHelper()
{
    modelHandler = NULL;
}
bool ARHelper::setup()
{
    ofxXmlSettings xml;
    if ( xml.loadFile("arconfig.xml"))
    {
        ofLog(OF_LOG_VERBOSE,"arconfig.xml loaded");
    }
    else
    {
        ofLog(OF_LOG_ERROR,"Could not Found arconfig.xml");
    }

    string xml_notice;
    xml.copyXmlToString(xml_notice);
    ofLog(OF_LOG_NOTICE,xml_notice);
    bDebug = xml.getValue("SETTINGS:DEBUG",false);
    useCustom= xml.getValue("SETTINGS:CUSTOMMARKER",false);
    int numPatternLoad= xml.getValue("SETTINGS:PATTERNLOAD",1);
    int numPatternDec= xml.getValue("SETTINGS:PATTERNDETECT",1);
    //bMirror =  xml.getValue("SETTINGS:MIRROR",false);
    float _far = xml.getValue("SETTINGS:FAR",5000.0f);
    float _near = xml.getValue("SETTINGS:NEAR",1.0f);
    north.loadImage("image/north.png");

    gui.addPage("AR Setting");
    gui.addToggle("Debug",bDebug);
    gui.addToggle("bMirror",bMirror);

    gui.addSlider("tol",tol,0,100);
    conf = 0;
    myFont.loadFont("fonts/verdana.ttf",24);
//#ifdef DEBUG
    //bDebug = true;
//#endif
    width = 640;
    height = 480;
//    displayW = ofGetWidth();
//    displayH =(displayW/4.0f)*3.0f;

    bpp = 1;





    //grabber.initGrabber(width, height);
    convert.allocate(width, height);
    gray.allocate(width, height);

    numPixels = width*height*bpp;
    cameraBuffer = new unsigned char[numPixels];

    bDraw = false;

    useBCH = false;



    // ----------------------------------  AR TK+ STUFF - ripped from the single marker demo app

    // create a tracker that does:
    //  - 6x6 sized marker images
    //  - samples at a maximum of 6x6
    //  - works with luminance (gray) images
    //  - can load a maximum of 10 patterns
    //  - can detect a maximum of 10 patterns in one image


#ifdef SINGE_MARKER
    if (useCustom)    tracker = new ARToolKitPlus::TrackerSingleMarkerImpl<16,16,16, numPatternLoad, numPatternDec>(width,height);
    else    tracker = new ARToolKitPlus::TrackerSingleMarkerImpl<6,6,6,numPatternLoad,numPatternDec>(width,height);
#else
    if (useCustom)    tracker = new ARToolKitPlus::TrackerMultiMarkerImpl<16,16,16, numPatternLoad,numPatternDec>(width,height);
    else    tracker = new ARToolKitPlus::TrackerMultiMarkerImpl<6,6,6, numPatternLoad,numPatternDec>(width,height);

#endif//#ifdef SINGE_MARKER
    const char* description = tracker->getDescription();
    printf("ARToolKitPlus compile-time information:\n%s\n\n", description);

    // set a logger so we can output error messages
    tracker->setLogger(&logger);
    tracker->setPixelFormat(ARToolKitPlus::PIXEL_FORMAT_LUM);
#ifdef SINGE_MARKER
    //if(useCustom)
    if (!tracker->init((const char *)ofToDataPath("LogitechPro4000.dat").c_str(),

                       _near, _far))
#else
    if ( !tracker->init(  (const char *)ofToDataPath("LogitechPro4000.dat").c_str(),
                          (const char *)ofToDataPath("markerboard_480-499.cfg").c_str(), _near,_far ))           // load std. ARToolKit camera file
#endif//#ifdef SINGE_MARKER
    {
        printf("ERROR: init() failed\n");
        delete cameraBuffer;
        delete tracker;
        std::exit(0);
        return false;
    }

    // define size of the marker
    //tracker->setPatternWidth(80); // I'm not sure how to define the size with multimarkers since it doesnt seem to have this option.

    // the marker in the BCH test image has a thin border...
    tracker->setBorderWidth(useBCH ? 0.125f : 0.250f);


    // set a threshold. alternatively we could also activate automatic thresholding
    //tracker->setThreshold(125);
    tracker->activateAutoThreshold(true);


    // let's use lookup-table undistortion for high-speed
    // note: LUT only works with images up to 1024x1024
    tracker->setUndistortionMode(ARToolKitPlus::UNDIST_LUT);

    // RPP is more robust than ARToolKit's standard pose estimator
    tracker->setPoseEstimator(ARToolKitPlus::POSE_ESTIMATOR_RPP);

    // switch to simple ID based markers
    // use the tool in tools/IdPatGen to generate markers

    //prot from single exmaple
    if (useCustom)
    {
        tracker->setMarkerMode (ARToolKitPlus::MARKER_TEMPLATE);
        ofxDirList dir;
        int numPatt = dir.listDir("patterns");
        for (int i = 0 ; i < numPatt ; i++)
        {
            char fn[256];
            size_t found;
            found = dir.getPath(i).find("patt");
            if (found != string::npos)
            {
                strcpy(fn, dir.getPath(i).c_str());
                ofLog(OF_LOG_VERBOSE,"Try to load pattern "+string(fn));
                tracker->arLoadPatt(fn);
            }
        }
        /*char fn[256];
        strcpy(fn,ofToDataPath("patt.hiro").c_str());
        ofLog(OF_LOG_VERBOSE,"Try to load pattern "+string(fn));
        tracker->arLoadPatt(fn);


        strcpy(fn,ofToDataPath("patt.kanji").c_str());
        ofLog(OF_LOG_VERBOSE,"Try to load pattern "+string(fn));
        tracker->arLoadPatt(fn);

        strcpy(fn,ofToDataPath("patt.yuco").c_str());
        ofLog(OF_LOG_VERBOSE,"Try to load pattern "+string(fn));
        tracker->arLoadPatt(fn);*/

    }
    else tracker->setMarkerMode(useBCH ? ARToolKitPlus::MARKER_ID_BCH : ARToolKitPlus::MARKER_ID_SIMPLE);


    if (modelHandler!=NULL)modelHandler->setup();
    ofLog(OF_LOG_VERBOSE,"AR HELPER SETUP COMPLETED");

    return true;
}
void ARHelper::update(unsigned char *pixels, int _width ,int _height)
{
    //grabber.grabFrame();
    bool bNewFrame = false;


    if (_width==width && _height==height)
    {
        //ofLog(OF_LOG_NOTICE,"ARHelper : convert  copy  pixels from camera!");
        //convert our camera frame to grayscale
        convert.setFromPixels(pixels, _width, _height);
        bNewFrame = true;
    }
    /*else if(_width!=0 && _height!=0)
    {
        ofLog(OF_LOG_WARNING,
        "ARHelper : ofImage  copy  pixels from camera! size "
        +ofToString(_width)+" "+ofToString(_height));
        ofImage temp;
        //temp.allocate( _width, _height, OF_IMAGE_COLOR);
        temp.setFromPixels(pixels, _width, _height, OF_IMAGE_COLOR);
        temp.resize(width,height);
        convert.setFromPixels(temp.getPixels(), width, height);
        bNewFrame = true;
    }*/
//    else
//    {
//        ofLog(OF_LOG_WARNING,
//        "ARHelper : copy  pixels ERROR! size "
//        +ofToString(_width)+" "+ofToString(_height));
//        bNewFrame = false;
//    }
    if ( bNewFrame)
    {
        //convert.setFromPixels(grabber.getPixels(), width, height);
        if (bMirror)convert.mirror(false, true);
        gray = convert;


#ifdef SINGE_MARKER
//find the marker and get back the confidence
        markerId = tracker->calc(gray.getPixels());
        conf = (float)tracker->getConfidence();

        if ( conf > 0.0 )
        {
            bDraw = true;
        }
        else bDraw = false;


//#ifdef DEBUG

        if (bDebug)
        {
            printf("\n\nFound marker %d  (confidence %d%%)\n\nPose-Matrix:\n  ", markerId, (int(conf*100.0f)));

            //prints out the matrix - useful for debugging?
            for (int i=0; i<16; i++)
                printf("%.2f  %s", tracker->getModelViewMatrix()[i], (i%4==3)?"\n  " : "");
        }
//#endif// #ifdef DEBUG
#else
        //find the marker and get back the confidence
        tracker->calc(gray.getPixels());
#endif// #ifdef SINGE_MARKER

    }
    if (modelHandler!=NULL)modelHandler->update();
}
void ARHelper::draw()
{
    ofSetColor(0xffffff);



    glViewport(0, 0, ofGetWidth(), ofGetHeight() );

    //else if(mode == FIT)convert.draw(0, 0,ofGetWidth(),ofGetHeight());
    //this is where we use the calculated matrix from ARToolkitPlus to put
    //in our graphics at the location and orientation of the marker.
    //- the matrix has the 0,0 point as the center of the marker.



    glMatrixMode( GL_PROJECTION );
    glLoadMatrixf(tracker->getProjectionMatrix());
    float m[ 16 ];
#ifdef SINGE_MARKER
    if (markerId !=-1)
    {

        for (int i=0; i<16; i++)
            m[i] =  tracker->getModelViewMatrix()[i];
        if (modelHandler!=NULL && int(conf*100.0f)>tol)modelHandler->updateModelMatrix(markerId , m,(float*) tracker->getProjectionMatrix());

    }
#else
    int numDetected = tracker->getNumDetectedMarkers();
    for (int i=0; i<numDetected; i++)
    {

        ARToolKitPlus::ARMarkerInfo marker = tracker->getDetectedMarker(i);

        float m34[ 3 ][ 4 ];
        float c[ 2 ] = { 0.0f, 0.0f };
        float w = 40.0f;


        tracker->rppGetTransMat( &marker, c, w, m34 );

        float m[ 16 ]; //this is some crazy matrix transformative stuff. I think initially it came out of one of the arToolkit functions.... but i got it from here: http://chihara.naist.jp/people/STAFF/imura/computer/OpenGL/artp/disp_content
        for ( int i = 0; i < 3; i++ )
        {
            for ( int j = 0; j < 4; j++ )
            {
                m[ j * 4 + i ] = m34[ i ][ j ];
            }
        }
        for ( int j = 0; j < 3; j++ )
        {
            m[ j * 4 + 3 ] = 0.0f;
        }
        m[ 3 * 4 + 3 ] = 1.0f;

        if (modelHandler!=NULL )modelHandler->updateModelMatrix(marker.id , m,(float*) tracker->getProjectionMatrix());

        //if (modelHandler!=NULL)modelHandler->updateModelMatrix(marker.id , m);
        debugDraw(marker.id,m);
    }
#endif //#ifdef SINGE_MARKER

    glEnable(GL_DEPTH_TEST);
    if (modelHandler!=NULL)modelHandler->draw();
    north.draw(-256,-256);
    glDisable(GL_DEPTH_TEST);

    glDisable(GL_DEPTH_TEST);
    debugDraw(markerId,m);
    glEnable(GL_DEPTH_TEST);

    GLdouble model_view[16];
    GLdouble projection[16];
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);




    for (int i=0; i<16; i++)
    {
        model_view[i] =  tracker->getModelViewMatrix()[i];
        projection[i] = tracker->getProjectionMatrix()[i];
    }
    if (modelHandler!=NULL)
        for (int i = 0 ; i<modelHandler->getModel().size() ; i++)
        {
            modelHandler->getModel()[i]->findScreenCoord(projection,viewport);
        }



    GLdouble ret_x, ret_y, ret_z;
    gluProject(0,0,0,
               model_view,
               projection,
               viewport, &ret_x, &ret_y, &ret_z);
    float x = ret_x;
    float y = ofGetHeight()-ret_y;
    markerV.set(x,y);
    ofSetupScreen();
    if (bDebug)
    {

        ofNoFill();
        ofSetColor(255,255,255,125);
        ofRect(-50+x,-50+y,100,100);

        ofRect(0,(ofGetHeight()/2)-IMAGE_AREA,ofGetWidth(),IMAGE_AREA*2);

    }

    {
        glDisable(GL_DEPTH_TEST);
        if (modelHandler!=NULL)modelHandler->draw2D();
        glEnable(GL_DEPTH_TEST);
    }
}
void ARHelper::videoSettings()
{
    //grabber.videoSettings();
}
void ARHelper::toggleMirror()
{
    bMirror = !bMirror;
}
void ARHelper::toggleDebug()
{
    bDebug = !bDebug;
}
void ARHelper::debugDraw(int id ,float *m)
{
    if (bDebug)
    {
        glMatrixMode( GL_MODELVIEW );
        glLoadMatrixf( m );
        ofSetColor(255 , 255, 255 );
        glPushMatrix();

        ofSetColor(255,0,0);
        ofxLine(-width,0,0,width,0,0);//xaxis
        ofDrawBitmapString("X axis",10,0);

        ofSetColor(0,0,255);
        ofxLine(0,-height,0,0,height,0);//yaxis
        ofDrawBitmapString("Y axis",0,10);

        ofSetColor(0,255,0);
        ofxLine(0,0,-width,0,0,width);//zaxis
        ofDrawBitmapString("Z axis",0,0);

        glPopMatrix();
        glPushMatrix();
        glRotatef(-90, 1, 0, 0);
        glRotatef(-90, 0, 1, 0);
        ofSetColor(255 , 0, 255 );
        string msg = ofToString(id)+"conf : "+ofToString((int(conf*100.0f)))+"%";
        myFont.drawString(msg, -myFont.stringWidth(msg)/2,0);
        glPopMatrix();
    }

}
