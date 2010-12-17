#pragma once
#include "ARModelCollections.h"
ARModelCollections::ARModelCollections()
{
    image_path = "images/";
    model_path = "models/";
    sound_path = "sounds/";
    //timeOut = 100;//frame
    timeOut = 2; //second
    fadeInCount = 100;
    fadeOutCount = 0;
    fadable.setAlpha(0);
    fadable.setFadeSeconds(2);
    lastFrame =  0;
    currentFrame = 0;
    exitCount=0;
    inactiveCount = 0;
    iseize =50;
    easing = 0.9;
    sndPlayer.bLoop = false;
	memset(targetMatrix, 0, 16);
	memset(m, 0, 16);

	scale = 1;
    tweenScale = 1;
    targetEasing = 0;
    //perlin = new ofxPerlin();
}
void ARModelCollections::setup()
{
    sndPlayer.loadSound("sounds/button.mp3");
    ofLog(OF_LOG_VERBOSE,"[ARModelCollections] Setting up ==== "+folder+"============ START");
    ofAddListener(ofEvents.mouseReleased, this, &ARModelCollections::_mouseReleased);
    gui.addPage(folder);
    gui.addSlider("Scale",scale,1,600);
    tweenScale = 2;

    gui.addSlider("tweenScale",tweenScale,1,10);

    gui.addSlider("Image Size",iseize,1,100);
    gui.addSlider("Easing",targetEasing,0.000001f,1.0f);
    if (model_path=="")
    {
        ofLog(OF_LOG_ERROR,"FileName is NULL");
        return;
    }
    ofxDirList dir;
    int num  = dir.listDir(model_path);
    ARModel* model;

    for (int i = 0 ; i < num ; i ++)
    {
        size_t found;
        size_t found2;
        found = dir.getPath(i).find(".3DS");
        found2 = dir.getPath(i).find(".3ds");
        if (found != string::npos)
        {
            model = new ARModel();
            ofDisableArbTex();
            model->loadModel(ofToDataPath(dir.getPath(i)).c_str(), 1);
            ofEnableArbTex();
            model->setup();
            model->setScale(scale,scale,scale);
            models.push_back(model);
            ofLog(OF_LOG_VERBOSE,"Loading Model "+dir.getPath(i)+"Successed");
        }
        else if (found2 != string::npos)
        {
            model = new ARModel();
            model->loadModel(ofToDataPath(dir.getPath(i)).c_str(), 1);
            model->setup();
            model->setScale(scale,scale,scale);
            models.push_back(model);
            ofLog(OF_LOG_VERBOSE,"Loading Model "+dir.getPath(i)+"Successed");
        }
    }
    num  = dir.listDir(model_path+"/images");
    int numPs = 0;
    for (int i = 0 ; i < num ; i ++)
    {
        size_t found = dir.getPath(i).find(".png");
        if (found != string::npos)
        {
            ARImage * img = new ARImage();
            img->loadImage(dir.getPath(i));
            model_images.push_back(img);
            //gui.addQuadWarper("Image "+ofToString(numPs),*model_images[numPs],model_images[numPs]->points);
            //numPs++;
            ofLog(OF_LOG_VERBOSE,"Loading Images "+dir.getPath(i)+"Successed");
        }
    }

	easing = 1;

    numModels = models.size();
    numModelImages = model_images.size();

    num  = dir.listDir(image_path);
    for (int i = 0 ; i < num ; i ++)
    {
        size_t found = dir.getPath(i).find(".png");
        if (found != string::npos)
        {
            ofImage * img = new ofImage();
            img->loadImage(dir.getPath(i));
            screen_images.push_back(img);
            ofLog(OF_LOG_VERBOSE,"Loading Images "+dir.getPath(i)+"Successed");
        }
    }
    numScreenImages = screen_images.size();
    selfSetup();


    ofLog(OF_LOG_VERBOSE,"[ARModelCollections] Setting up ==== "+folder+"============ END\n\n\n");

}
void ARModelCollections::update()
{

    fadable.updateFade();
    currentFrame = ofGetElapsedTimef();
    if (ofGetElapsedTimef()>startGameTime)
    {
        if(currentFrame-lastFrame>0)
        {
            fadable.fadeOut();
        }
        if (matrixUpdated )
        {
            fadable.fadeIn();

            lastFrame = currentFrame;
            exitCount++;
            fadeOutCount = 0;



        }
        if (currentFrame-lastFrame<timeOut)
        {
             selfUpdate();
            if (exitCount==fadeInCount)
            {
                fadeIn();
				easing = targetEasing;
                if (sndPlayer.bLoadedOk)sndPlayer.play();

            }
            if (exitCount==1)
            {
                easing = 1;
                static ofAREventArgs arEventArgs;
                arEventArgs.markerId = id;
                ofNotifyEvent(MODEL_ACTIVE, arEventArgs, this);


            }
        }
        else if (currentFrame-lastFrame==timeOut || !matrixUpdated)
        {
            fadeOutCount++;
            if (fadeOutCount==1)

            {
                fadeOut();
                static ofAREventArgs arEventArgs;
                arEventArgs.markerId = -1;
                ofNotifyEvent(MODEL_DEACTIVE, arEventArgs, this);
                if (sndPlayer.bLoadedOk)sndPlayer.stop();
            }
        }
    }
    for (int i = 0 ; i < 16 ; i++)
        targetMatrix[i] -= (targetMatrix[i]-m[i])*easing;
    for (int i = 0 ; i < numModels ; i ++)
    {

        models[i]->setScale(scale,scale,scale);

    }


    matrixUpdated = false;

};
void ARModelCollections::draw()
{
    if (currentFrame-lastFrame<timeOut && ofGetElapsedTimef()>startGameTime)
    {

        glMatrixMode( GL_MODELVIEW );
        glLoadMatrixd( targetMatrix );
        //glLoadMatrixf( m );
        ofSetColor(255 , 255, 255 ,10);//255*fadable.getAlpha());
        glEnable(GL_DEPTH_TEST);
        ofEnableAlphaBlending();
        drawModel();
        glDisable(GL_DEPTH_TEST);
        ofDisableAlphaBlending();

    }

};
void ARModelCollections::draw2D()
{
    if (currentFrame-lastFrame<timeOut && ofGetElapsedTimef()>startGameTime)
    {
        ofSetColor(255,255,255);
        ofEnableAlphaBlending();

        glBlendFunc(GL_SRC_ALPHA,GL_ONE);
        float e = tweenexpo.update();

        if (e>0.9)
        {
            ofPushStyle();
            ofSetColor(255,255,255,255*e);
            for (int i = 0 ; i < numModelImages ; i ++)
            {

                float _x = models[i%numModels]->getPos2d()->x;
                float _y = models[i%numModels]->getPos2d()->y;
                model_images[i]->draw(_x,_y);
            }
            ofPopStyle();

            ofDisableAlphaBlending();



        }

        ofEnableAlphaBlending();
        ofPushStyle();
        ofSetColor(255,255,255,255*e);
        for (int i = 0 ; i < numScreenImages ; i ++)
        {
            screen_images[i]->draw(0,0);
        }
        ofPopStyle();
        ofDisableAlphaBlending();




    }

}
void ARModelCollections::_mouseReleased(ofMouseEventArgs &e)
{
    for (int i = 0 ; i < numModels ; i ++)
    {
        models[i]->setScale(scale,scale,scale);
    }
}

void ARModelCollections::drawModel()
{
    ofSetColor(255,0,255);

    for (int i = 0 ; i < numModels ; i ++)
    {

        models[i]->draw();

    }

}

void ARModelCollections::setModelMatrix(int  _id ,float *_m , float *project)
{
    if (_id==id)
    {
        for (int i = 0 ; i < 16 ; i++)
            m[i] = _m[i];
        matrixUpdated = true;
        inactiveCount = 0;
    }
    else
	{
	    inactiveCount++;
#ifdef	SINGE_MARKER
		lastFrame = 0;
#endif
	}

	if(_id!=-1 && _id!=id && inactiveCount==1)
    {

        static ofAREventArgs arEventArgs;
        arEventArgs.markerId = id;
        ofNotifyEvent(MODEL_DEACTIVE, arEventArgs, this);
    }
    /*else if (inactiveCount==1)
    {

        static ofAREventArgs arEventArgs;
        arEventArgs.markerId = id;
        ofNotifyEvent(MODEL_DEACTIVE, arEventArgs, this);
    }*/
}
void ARModelCollections::findScreenCoord( const GLdouble *proj, const GLint *view)
{
    GLdouble ret_x, ret_y, ret_z;
    float x = 0;
    float y = 0;
    for (int i = 0 ; i < numModels ; i ++)
    {



        gluProject( models[i]->getPos3d()->x,
                    models[i]->getPos3d()->y,
                    models[i]->getPos3d()->z,
                    targetMatrix,//get the modle matrix from modelset to calc each model screen coord
                    proj,
                    view, &ret_x, &ret_y, &ret_z);
        x = ret_x;
        y = ofGetHeight()-ret_y;
        models[i]->setARModelPosition2D(ofxVec2f(x,y));
    }
}
