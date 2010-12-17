#include "PBModel.h"

PBModel::PBModel(int _id,float _scale, string _folder)
{
    ARModelCollections::ARModelCollections();
    id = _id;
    scale = _scale;//0.1;

    folder = _folder;
    model_path = model_path+folder;
    image_path = image_path+folder;
    z = 0;
}

PBModel::~PBModel()
{

}
void PBModel::selfSetup()
{

    gui.addSlider("Model Z",z,-500,500);
    gui.addSlider("RoationX",rotX,0,360);
    gui.addSlider("RoationY",rotY,0,360);
    gui.addSlider("RoationZ",rotZ,0,360);
    gui.addSlider("Alpha",alpha,0,255);
    gui.addToggle("debug",debug);

    ofxXmlSettings xml;
    if ( xml.loadFile("arconfig.xml"))
    {
        ofLog(OF_LOG_VERBOSE,"arconfig.xml loaded");
    }
    else
    {
        ofLog(OF_LOG_ERROR,"Could not Found arconfig.xml");
    }
    int m_range = xml.getValue("SETTINGS:MODEL_POSITION_RANDGE",200);
    int i_range = xml.getValue("SETTINGS:IMAGE_POSITION_RANDGE",200);


    //gui.addPage(folder+"_models");
    modelPnts = new ofPoint[numModels];
    for (int i = 0 ; i < numModels ; i ++)
    {
        gui.addSlider2d("Point "+ofToString(i),modelPnts[i],-m_range,m_range,-m_range,m_range);
    }

    //gui.addPage(folder+"_Images");
    points = new ofPoint[numModelImages];

    //gui.addSlider("pointZ",pointZ,-100,100);
    for (int i = 0 ; i < numModelImages ; i ++)
    {
        gui.addSlider2d("Point "+ofToString(i),points[i],-i_range,i_range,-i_range,i_range);
    }
    gui.loadFromXML();

}
void PBModel::selfUpdate()
{

    for (int i = 0 ; i < numModels ; i ++)
    {
        models[i]->setARModelPosition3D(ofxVec3f(modelPnts[i].x,modelPnts[i].y,tweenScale*scale*(i)*(tweenexpo.update()/numModels)));
    }
}
void PBModel::fadeIn()
{
    unsigned duration = 5000;
    unsigned delay = 5;
    int target = 1;

    tweenexpo.setParameters(7,easingexpo,ofxTween::easeOut,0,target,duration,delay);

}
void PBModel::fadeOut()
{
    unsigned delay = 1;
    unsigned duration = 5000;
    int target = 0;
    exitCount = 0;

    tweenexpo.setParameters(7,easingexpo,ofxTween::easeIn,target,0,duration,delay);

}
void PBModel::drawModel()
{



//glActiveTexture(GL_TEXTURE0);
    glEnable(GL_DEPTH_TEST);
    glPushMatrix();

    glTranslatef(0,0,z);
    glRotatef(-90,0,0,1);
    glRotatef(rotX,1,0,0);
    glRotatef(rotY,0,1,0);
    glRotatef(rotZ,0,0,1);

    for (int i = 0 ; i < numModels ; i ++)
    {
        models[i]->draw();
    }
    if(debug)
    {
        ofDrawBitmapString("Tween Value: "+ofToString(tweenexpo.update()),0,0);
    }

    glPopMatrix();

    //ofSetupScreen();

};
void PBModel::draw2D()
{
    if (currentFrame-lastFrame<timeOut && ofGetElapsedTimef()>startGameTime)
    {
        ofSetColor(255,255,255);
        ofEnableAlphaBlending();

        glBlendFunc(GL_SRC_ALPHA,GL_ONE);
        float e = tweenexpo.update();

        if (e>0.9)
        {
            ofSetColor(255,255,255,255*e);
            for (int i = 0 ; i < numModelImages ; i ++)
            {
                model_images[i]->draw(0,0);
            }
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
void PBModel::findScreenCoord( const GLdouble *proj, const GLint *view)
{
    GLdouble ret_x, ret_y, ret_z;
    float x = 0;
    float y = 0;
    for (int i = 0 ; i < numModelImages ; i ++)
    {
        gluProject( models[i%numModels]->getPos3d()->x + points[i].x,
                    models[i%numModels]->getPos3d()->y + points[i].y,
                    models[i%numModels]->getPos3d()->z +z,
                    targetMatrix,//get the modle matrix from modelset to calc each model screen coord
                    proj,
                    view, &ret_x, &ret_y, &ret_z);
        x = ret_x;
        y = ofGetHeight()-ret_y;
        //models[i]->setARModelPosition2D(ofxVec2f(x,y));
        model_images[i]->setPos(x,y);
    }
}
