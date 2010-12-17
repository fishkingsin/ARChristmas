#include "testApp.h"
#include "PBModel.h"
#include <stdio.h>
static const string cap_path = "captures/";
//--------------------------------------------------------------
void testApp::setup()
{


    myFont.loadFont("fonts/verdana.ttf", 24);
    ofBackground(0,0,0);

    if ( xml.loadFile("config.xml"))
    {
        ofLog(OF_LOG_VERBOSE,"config.xml loaded");
    }
    else
    {
        ofLog(OF_LOG_ERROR,"Could not Found arconfig.xml");
    }

    bDebug = xml.getValue("SETTINGS:DEBUG",false);
    if (bDebug)
    {
        ofSetLogLevel(OF_LOG_VERBOSE);

    }
    else
    {
        ofSetLogLevel(OF_LOG_ERROR);

    }
    countDown.setup(xml.getValue("SETTINGS:COUNTDOWN",3));

    ofHideCursor();
    //=======================
    //delete captures storage
    MAX_CAP_FILE = xml.getValue("SETTINGS:MAX_CAPTURES",200);
    sendImagesFile  = xml.getValue("SETTINGS:IMAGE_NAME","image.jpg");
    clearStorage();
    //=======================
    int width = xml.getAttribute("SETTINGS:CANVAS","width",1920);
    int height = xml.getAttribute("SETTINGS:CANVAS","height",1080);
    int camW = xml.getAttribute("SETTINGS:CAMERA","width",640);
    int camH = xml.getAttribute("SETTINGS:CAMERA","height",480);
    useAR = xml.getValue("SETTINGS:AR",false);
    x = xml.getAttribute("SETTINGS:CANVAS","x",0);
    y = xml.getAttribute("SETTINGS:CANVAS","y",0);

    gui.addSlider("timeout",timeout,0.00001f,10.0f);// = xml.getAttribute("SETTINGS:SCREENSAVER","timeout",1);
    gui.addSlider("maskA",maskA,1,255);
    gui.addButton("bCapture",bCapture);
    gui.addToggle("fgLoaded",fgLoaded);
    //gui.addToggle("bFroeGoundPalyer",bFroeGoundPalyer);
    gui.addToggle("LoopVideo",bLoop);
    gui.addSlider("vidScale",vidScale,0,2);
    gui.addSlider("Movie_X",mx,-width/2,width/2);
    gui.addSlider("Movie_y",my,-height/2,height/2);
    bFroeGoundPalyer = false;

    setUpInstructionVideo();




    ofSetWindowShape(width,height);
    //bWideScreen = false;
    /*if (((float)width/(float)height)>(4.0f/3.0f))
    {
        bWideScreen = true;
    }*/
    ofSetFullscreen(xml.getValue("SETTINGS:FULLSCREEN",false));
    ofSetWindowPosition(x,y);
    ofSetVerticalSync(true);
    ofEnableSmoothing();
    ofEnableAlphaBlending();



    camera.setup(camW,camH);
    camera.setDisplaySize(camW,camH);


    fboWidth = xml.getAttribute("SETTINGS:FBOSAVER","width",ofGetWidth());
    fboHeight = xml.getAttribute("SETTINGS:FBOSAVER","height",ofGetHeight());
    fboSaver.allocate(fboWidth,fboHeight,true);

    //Network------------------------
    //NETWORK SETTINGS

    network.setup();
    bChangeBG=bShow=false;
    //Network------------------------END

    bCapture = false;
    bSetup = false;

    loading.loadImage("images/loading.png");
    intro.loadImage("images/intro.png");


    fadable.setAlpha(1);
    fadable.setFadeSeconds(1);

    gui.loadFromXML();
    fadable.fadeOut();
    timer.setup(60*timeout,false);
    ofAddListener(countDown.COUNTER_REACHED,this,&testApp::Shot);
    //ofAddListener(timer.TIMER_REACHED,this,&testApp::fadeIn);
    //ofAddListener(network.MESSAGE_AVAILABLE,this,&testApp::networkMessage);
    //ofAddListener(fboSaver.imageSaver.SAVE_DONE,this,&testApp::afterSave);
    startThread(true,false);
    fgLoaded = fgImg.loadImage("images/foreground/fg_number0.png");
    tempCap.allocate(ofGetWidth(),ofGetHeight(),OF_IMAGE_COLOR_ALPHA);
    num_of_fg = 0;
    bChangeFG = false;
    activeId = -1;

    loadingMask.setFadeSeconds(xml.getValue("SETTINGS:FADER",2));
    pm.setup("images/lips.png");
    bKiss = -1;

}
void testApp::threadedFunction()
{
    while ( isThreadRunning() != 0 )
    {
        if ( lock() )
        {
            unlock();

            ofSleepMillis(1 * 1000);
        }

    }
}
//--------------------------------------------------------------
void testApp::update()
{
    //ARModelCollections *activeModel = NULL;
    pm.update();

    if (ofGetFrameNum()==10)
    {


        if (!bSetup && useAR)
        {
            arhelper = new ARHelper();
            modelHandler = new ModelHandler();
            arhelper->setModelHandler(modelHandler);

            bSetup = arhelper->setup();


            if (xml.pushTag("SETTINGS"))
            {
                int tags = xml.getNumTags("MODELS");
                for (int i = 0 ; i <  tags ; i++)
                {
                    int id = xml.getAttribute("MODELS","id",0,i);
                    float scale = xml.getAttribute("MODELS","scale",1.0f,i);
                    string fn = xml.getAttribute("MODELS","foldername","",i);
                    PBModel *pbModel = new PBModel(id,scale,fn);
                    pbModel->setup();
                    glutSwapBuffers();
                    modelHandler->addModelCollection((ARModelCollections*)pbModel);
                }


                for (int  i = 0 ; i < modelHandler->getModel().size();i++)
                {
                    ARModelCollections* armodel = modelHandler->getModel()[i];
                    ofAddListener(armodel->MODEL_ACTIVE,this,&testApp::arModelActived);
                    ofAddListener(armodel->MODEL_DEACTIVE,this,&testApp::arModelDeactived);
                }
                xml.popTag();
            }
            gui.loadFromXML();
            //minimap.enabled = true;

            //bSetup = true;

        }

    }
    camera.update();
    if (bChangeBG)
    {
        string fn = network.getBackground();
        if (fn!="")
            camera.chageBackground(fn);
        network.sendMessage("background_changed");
        bChangeBG = false;
    }
    if (bChangeFG)
    {
        loadingMask.setAlpha(1);
        string fn = network.getForeground();
        if (fn!="")
        {
            ofDisableArbTex();
            fgLoaded=fgImg.loadImage(fn);
            ofEnableArbTex();
        }
        ofLog(OF_LOG_WARNING,"loadMovie "+ofToString(activeId) +" ");
        loadMovie(activeId);

        bChangeFG = false;
    }
    if (!bChangeFG)
    {
        loadingMask.updateFade();
        //loadingMask.fadeOut();
        loadingMask.setAlpha(0);
    }
    if (camera.getPixels()!=NULL && useAR && bSetup)
        arhelper->update(camera.getPixels(),camera.getWidth(),camera.getHeight());
    ofSoundUpdate();
    fadable.updateFade();
    if (useAR)
        if (bSetup && arhelper->isDetected())
        {

            timer.setup(timeout*60,false);
            timer.startTimer();
        }

    if (fadable.getAlpha()>0.9f || bFroeGoundPalyer)
    {
        if (bLoaded)
        {

            if (!bLoop && player.getCurrentFrame()==10)
            {
                //player.setPaused(true);
            }
            else
            {
                //player.setPaused(false);
                player.update();
                //printf("player.getCurrentFrame = %i\n",player.getCurrentFrame());
                if (bKiss>-1 && player.getCurrentFrame()==53)
                {
                    pm.addParticle();
                }
            }
#ifdef INSTRUCTION
            unsigned char *imgPixels = player.getPixels();
            //myTexture.loadData(imgPixels, vw,vh, GL_RGB);

            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, player.getTextureReference().getTextureData().textureID);


            // go back to default texture unit !!
            glActiveTexture(GL_TEXTURE0);
#endif//#ifdef INSTRUCTION
        }
    }

    if (canLoadImage)
    {
        ofDisableArbTex();
        bShow = lastCapImage.loadImage(cap_path+lastCaptureFile);
        ofEnableArbTex();
        ofLog(OF_LOG_VERBOSE,"lastCapImage load "+lastCaptureFile+": "+ofToString(bShow));
        canLoadImage = false;
        //bShow=  true;
    }

    //system("./yucolabPhotoBooth.exe > log`date +%Y%m%d%H%m%S`.txt");
}

//--------------------------------------------------------------
void testApp::draw()
{

    ofBackground(0,0,0);
    fboSaver.begin();
    ofPushStyle();
    ofSetColor(0,0,0);
    ofRect(0,0,ofGetWidth(),ofGetHeight());
    ofPopStyle();
    glPushMatrix();
    camera.draw(0,0,fboSaver.getWidth(),fboSaver.getHeight());
    glPopMatrix();
    if (bSetup && useAR)
    {


        arhelper->draw();
        if (bText)
        {


            glPushMatrix();
            ofSetColor(255 , 255, 255 );
            string msg = "";;
            myFont.drawString(msg, (ofGetWidth()/2)-myFont.stringWidth(msg)/2,ofGetHeight()/2);
            glPopMatrix();
        }
        ofSetupScreen();
        glDisable(GL_DEPTH_TEST);
        if (fadable.getAlpha()>0 || bFroeGoundPalyer)
        {
            ofPushStyle();
            ofEnableAlphaBlending();
            ofSetColor(0,0,0,maskA*fadable.getAlpha());
            ofRect(0,0,ofGetWidth(),ofGetHeight());



            if (bLoaded)
            {
#ifdef INSTRUCTION
                int cx = ofGetWidth()/2;
                int cy = ofGetHeight()/2;
                ofSetColor(255,255,255);
                float _w = vw*vidScale;
                float _h = vh*vidScale;
                shaderBGSub.setShaderActive(true);
                shaderBGSub.setUniform("tex1", 1);
                shaderBGSub.setUniform("tex2", 2);
                shaderBGSub.setUniform("tol", tol);
                player.draw(mx+cx-_w/2,my+cy-_h/2,_w,_h);


                myTexture2.unbind();
                myTexture.unbind();

                shaderBGSub.setShaderActive(false);
                glDisable(GL_DEPTH_TEST);
#endif //#ifdef INSTRUCTION
            }

            ofDisableAlphaBlending();
            ofPopStyle();
        }
        ofSetColor(0xffffff);
        pm.draw(bKiss);

    }
    else if (useAR)
    {
        ofEnableAlphaBlending();
        ofSetColor(255,255,255,255*sinf(ofGetElapsedTimef())+125);
        loading.draw(0,0);
        ofDisableAlphaBlending();
        ofDrawBitmapString("Loading ... ",ofGetWidth()/2,ofGetHeight()/2);
    }
    if (fgLoaded)
    {
        ofEnableAlphaBlending();
        fgImg.draw((fboWidth-ofGetWidth())/2.0f,(fboHeight-ofGetHeight())/2.0f,
                   ofGetWidth(),ofGetHeight());
        ofDisableAlphaBlending();
    }
    //mask

    if (bCapture)
    {
        char buf[256];
        sprintf(buf,"%i_%i_%i_%i_%i_%i.jpg",
                ofGetYear(),
                ofGetMonth(),
                ofGetDay(),
                ofGetHours(),
                ofGetMinutes(),
                ofGetSeconds());
        lastCaptureFile = buf;
//        fboSaver.getImageSaver()->resize(ofGetWidth(),((ofGetWidth()/3.0f)*2.0f));
        fboSaver.saveImage(cap_path+lastCaptureFile ,fboWidth,fboHeight);//need to get fbo pixel in GL_RGB and order RGBPixel
        fboSaver.saveImage(cap_path+sendImagesFile ,fboWidth,fboHeight);

        network.sendMessage(lastCaptureFile);
        //bool bLoad = false;




    }

    fboSaver.end();
    //glActiveTexture(GL_TEXTURE0);
    //glDisable(GL_DEPTH_TEST);
    //for 16:9 screen

    fboSaver.draw((ofGetWidth()-fboWidth)/2.0f,(ofGetHeight()-fboHeight)/2.0f,fboWidth,fboHeight);;
    ofSetColor(0xffffff);
    if (bCapture)
    {
        tempCap.clear();
        tempCap.grabScreen(0,0,ofGetWidth(),ofGetHeight());
#ifdef DEBUG
        tempCap.saveJpg("captures/test.jpg",50);
#endif
        bShow = true;
        bCapture = false;
    }
    if (bShow)tempCap.draw(0,0,ofGetWidth(),ofGetHeight());
    countDown.draw();
    gui.draw();
    //if (bChangeFG)
    {

        if (bChangeFG)
        {
            ofDrawBitmapString("Loading ... ",ofGetWidth()/2,ofGetHeight()/2);
        }
    }

}

//--------------------------------------------------------------
void testApp::keyPressed(int key)
{
    if (bDebug)
    {
        switch (key)
        {

        case 's':
            if (useAR)arhelper->videoSettings();
            break;
        case 'd':
            if (useAR)arhelper->toggleDebug();
            break;
        case 'f':
            ofToggleFullscreen();
            break;
        case ' ':
            gui.toggleDraw();
            if (gui.isOn())
            {
                ofShowCursor();
                ofSetWindowPosition(0,0);
            }
            else
            {
                ofHideCursor();
                ofSetWindowPosition(x,y);
            }
            break;
        case '[':
            gui.nextPage();
            break;
        case ']':
            gui.prevPage();
            break;
        case '`':
            gui.nextPageWithBlank();
            break;
        case 'l':
            gui.setAlignRight(true);
            break;
        case 'L':
            gui.setAlignRight(false);
            break;

        case 'm':
            if (useAR)arhelper->toggleMirror();
            break;
        case'0':
        case'1':
        case'2':
        case'3':
        case'4':
            closeMovie();
            num_of_fg = key-'0';

            if (num_of_fg<movie_list.size())
            {
                string fn = movie_list[num_of_fg];
                bLoaded = player.loadMovie(fn);
            }

            break;
        case OF_KEY_F1:
            glFrontFace(GL_CW);
            break;
        case OF_KEY_F2:
            glFrontFace(GL_CCW);
            break;
        case OF_KEY_F3:
            player.setLoopState(OF_LOOP_NONE);
            break;
        case OF_KEY_F4:
            player.setLoopState(OF_LOOP_NORMAL);
            break;
        case OF_KEY_F5:
            pm.addParticle();
            bKiss=1;
            break;
        case 'P':
            player.setPaused(true);
            break;
        case 'p':
            player.setPaused(false);
            break;
        }
    }

    switch (key)
    {


    case OF_KEY_RETURN:
        countDown.start();

        break;

    case OF_KEY_BACKSPACE:
        closeMovie();
        bShow = false;
        break;

        case OF_KEY_F1:
            pm.addParticle();

            break;




    }

}

//--------------------------------------------------------------
void testApp::keyReleased(int key)
{


}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y )
{

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button)
{

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button)
{
    camera.mousePressed( x, y, button);
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button)
{

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h)
{

}

void testApp::fadeIn(ofEventArgs &e)
{
    fadable.fadeIn();
    if (bLoaded)
    {
#ifdef TARGET_WIN32
        player.setFrame(1);
#endif
        player.setPaused(false);

    }
    timer.stopTimer();
    //minimap.setCandidate(-1);
    //minimap.enabled = false;
    //minimap.enabled = false;
    //ofLog(OF_LOG_VERBOSE,"testApp Timer Event --- fade In screen saver");
}
void testApp::arModelActived(ofAREventArgs &e)//armodels active
{
    if (ofGetFrameNum()>200)
    {
        //minimap.enableAppEvents();
        fadable.fadeOut();
//    if (bLoaded)player.setPaused(bLoop);
        //minimap.setCandidate(e.markerId);
        //minimap.enabled = true;
        timer.setTimer(60*timeout);
        timer.startTimer();

        ofLog(OF_LOG_VERBOSE,"testApp AR Marker Event --- fade Out screen saver id:"+ofToString(e.markerId));
        bText = false;
        if (activeId != e.markerId)
        {
            activeId = e.markerId;
            bChangeFG = true;
        }
        bFroeGoundPalyer = true;
    }

}
void testApp::arModelDeactived(ofAREventArgs &e)//armodel deactive
{
    if (ofGetFrameNum()>200)
    {
        ofLog(OF_LOG_VERBOSE,"testApp arModelDeactived id:"+ofToString(e.markerId));
        //minimap.enabled = false;
        bText = true;
        if (-1 == e.markerId)
        {
            activeId = e.markerId;
            closeMovie();
        }
    }
}
void testApp::setUpInstructionVideo()
{
    ofxDirList dir;
    dir.allowExt("wmv");
    dir.allowExt("mov");
    dir.allowExt("mp4");
    dir.setVerbose(true);
    int num = dir.listDir("movies/foreground");
    for (int i = 0 ; i < num ; i++)
    {
        movie_list.push_back(dir.getPath(i));
    }
    gui.addSlider("No. Foreground",num_of_fg,0,num-1);
    gui.addButton("Change Foregound",fgChange);
#ifdef INSTRUCTION

    bLoaded = player.loadMovie(xml.getAttribute("SETTINGS:SCREENSAVER","file","movies/instruction.mov"));

    ofDisableArbTex();
    secondImage.loadImage(xml.getAttribute("SETTINGS:SCREENSAVER","mask","images/green.jpg"));
    ofEnableArbTex();

    shaderBGSub.loadShader("shaders/mix");

    if (bLoaded)
    {

        ofDisableArbTex();
        player.play();
        player.setPaused(true);
        bool _loop = xml.getAttribute("SETTINGS:SCREENSAVER","loop",true);
        if (_loop)
            player.setLoopState(OF_LOOP_NORMAL);
        else
            player.setLoopState(OF_LOOP_NONE);

        vw = player.getWidth();
        vh = player.getHeight();
        secondImage.resize(vw,vh);

        unsigned char *imgPixels = player.getPixels();

        myTexture.allocate(vw,vh,GL_RGB);
        myTexture.loadData(imgPixels, vw,vh, GL_RGB);


        unsigned char *imgPixels2 = secondImage.getPixels();
        myTexture2.allocate( vw , vh , GL_RGB);
        myTexture2.loadData(imgPixels2, vw , vh , GL_RGB);

        ///// multitextureLoad stuff
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, myTexture.getTextureData().textureID);

        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, myTexture2.getTextureData().textureID);

        // go back to default texture unit !!
        glActiveTexture(GL_TEXTURE0);

        ofEnableArbTex();

        gui.addSlider("Shader Tolorence",tol,0.0f,1.0f);
        closeMovie();
    }
    else
    {

        //std::exit(0);
        ofLog(OF_LOG_FATAL_ERROR,"Can not Load model instructioon movie!");
    }
#endif//#ifdef INSTRUCTION
}
void testApp::networkMessage(OSCEventArgs &e)
{
    ofLog(OF_LOG_VERBOSE,"networkMessage Events");
    size_t emailFound = e.arg.find("mail");
    size_t changeBGfound = e.arg.find("bg_number");
    size_t changeFGfound = e.arg.find("fg_number");
    if (e.arg=="take_photo")
    {
        bCapture = true;
        camera.takePhoto();
        ofLog(OF_LOG_VERBOSE,"%s : TakePhoto",LogTime());
    }
    else if ( e.arg=="restart")
    {
        bShow = false;
        ofLog(OF_LOG_VERBOSE,"%s : Restart",LogTime());
        //network.sendMessage("restarted");
    }
    else if (e.arg=="calibrate")
    {
        ofLog(OF_LOG_VERBOSE,"%s : Calibrating",LogTime());
        camera.toggleCapBG();
    }
    else if (lastCaptureFile!="" && emailFound!=string::npos)
    {
        sendMail(e.arg);
    }
    else if (changeBGfound!=string::npos)
    {
        ofLog(OF_LOG_VERBOSE,LogTime());
        bChangeBG = true;
    }
    else if (changeFGfound!=string::npos)
    {
        ofLog(OF_LOG_VERBOSE,LogTime());
        bChangeFG = true;
    }
    else
    {
        ofLog(OF_LOG_VERBOSE,LogTime());
        network.sendMessage("What the fuck you are saying?");
    }




}
void testApp::afterSave(ofEventArgs &e)
{
    ofLog(OF_LOG_VERBOSE,"Image Saver done");
    network.sendMessage("data/"+cap_path+lastCaptureFile);
    canLoadImage = true;
}

void testApp::clearStorage()
{

    ofxDirList dir;
    dir.allowExt("png");
    dir.allowExt("jpg");
    dir.setVerbose(true);
    int num = dir.listDir("captures");
    ofLog(OF_LOG_VERBOSE,"Total "+ofToString(num)+" Files in captures ");

    if (num>MAX_CAP_FILE)
    {
        for (int i =0 ; i< num-MAX_CAP_FILE; i++)
        {
            if ( remove( dir.getPath(i).c_str()) == -1 )
                ofLog(OF_LOG_ERROR, "Error deleting "+dir.getPath(i) );
            else
                ofLog(OF_LOG_NOTICE, "Deleted "+dir.getPath(i) );
        }
    }
}
void testApp::sendMail(string msg)
{

    string addr = msg.substr (5,msg.length()-5);
    char cmd[MAXPATH];
    ofLog(OF_LOG_VERBOSE,"Send mail at %s",LogTime());
    string myPath = ofToDataPath(cap_path+sendImagesFile,true);
    std::replace( myPath.begin(), myPath.end(), '/', '\\' );
    sprintf(cmd," blat -bodyF body.txt -8bitmime -t %s -attach %s -sf subject.txt -debug",
            addr.c_str(),
            myPath.c_str()
           );
    ofLog(OF_LOG_VERBOSE,cmd);
    system(cmd);
}
void testApp::Shot(ofEventArgs &arg)
{
    bCapture = true;
}
void testApp::loadMovie(int id)
{
    closeMovie();
    if (xml.pushTag("SETTINGS"))
    {

        int tags = xml.getNumTags("MODELS");
        for (int i = 0 ; i <  tags ; i++)
        {
            int _id = xml.getAttribute("MODELS","id",0,i);
            if (_id==id)
            {

                string fn = xml.getAttribute("MODELS","video"," ",i);
                bLoaded = player.loadMovie(fn);
                bKiss = xml.getAttribute("MODELS","kiss",-1,i);
                if (bLoaded)
                {
                    player.setPaused(false);
                }


            }

        }
        xml.popTag();
    }
}
void testApp::closeMovie()
{
    if (bLoaded)
    {
        player.stop();
        player.closeMovie();
    }
    bLoaded = false;
}
