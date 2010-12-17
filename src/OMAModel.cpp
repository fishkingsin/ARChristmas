#include "PBModel.h"

PBModel::PBModel(int _id,float _scale, string _folder)
{
    ARModelCollections::ARModelCollections();
    id = _id;
    scale = _scale;//0.1;

    folder = _folder;
    model_path = model_path+folder;
    image_path = image_path+folder;

}

PBModel::~PBModel()
{

}
void PBModel::selfSetup()
{

    secondImage.loadImage("images/green.jpg");


    shaderBGSub.loadShader("shaders/mix");


    bLoaded = player.loadMovie("movies/"+folder+".mov");

    if (bLoaded)
    {

        ofDisableArbTex();
        player.play();
        player.setLoopState(OF_LOOP_NORMAL);
        player.setPaused(true);



        w = player.getWidth();
        h = player.getHeight();
        secondImage.resize(w,h);

        unsigned char *imgPixels = player.getPixels();
        myTexture.allocate(w,h,GL_RGB);
        myTexture.loadData(imgPixels, w,h, GL_RGB);




        unsigned char *imgPixels2 = secondImage.getPixels();
        myTexture2.allocate( w , h , GL_RGB);
        myTexture2.loadData(imgPixels2, w , h , GL_RGB);




        ///// multitextureLoad stuff
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, myTexture.getTextureData().textureID);



        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, myTexture2.getTextureData().textureID);


        // go back to default texture unit !!
        glActiveTexture(GL_TEXTURE0);

        ofEnableArbTex();


        gui.addSlider("Shader Tolorence",tol,0.0f,1.0f);
        gui.addSlider("Model Z",z,-500,500);

        gui.addSlider2d("Point 1",vidPoint[0],-1000,1000,-1000,1000);
        gui.addSlider2d("Point 2",vidPoint[1],-1000,1000,-1000,1000);
        gui.addSlider2d("Point 3",vidPoint[2],-1000,1000,-1000,1000);
        gui.addSlider2d("Point 4",vidPoint[3],-1000,1000,-1000,1000);

        //gui.addQuadWarper("MVVideo",player,vidPoint);
    }
    else
    {

        //std::exit(0);
        ofLog(OF_LOG_FATAL_ERROR,"Can not Load model "+folder+" movie!");
    }


    gui.addPage(folder+" models");
    modelPnts = new ofPoint[numModels];
    for (int i = 0 ; i < numModels ; i ++)
    {
        gui.addSlider2d("Point "+ofToString(i),modelPnts[i],-200,200,-200,200);
    }

    gui.addPage(folder+" Images");
    points = new ofPoint[numModelImages];

    //gui.addSlider("pointZ",pointZ,-100,100);
    for (int i = 0 ; i < numModelImages ; i ++)
    {
        gui.addSlider2d("Point "+ofToString(i),points[i],-200,200,-200,200);
    }

}
void PBModel::selfUpdate()
{

    if (bLoaded)
    {
        player.update();

        unsigned char *imgPixels = player.getPixels();
        myTexture.loadData(imgPixels, w,h, GL_RGB);


        ///// multitextureLoad stuff
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, myTexture.getTextureData().textureID);

        // go back to default texture unit !!
        glActiveTexture(GL_TEXTURE0);
    }
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
    if (bLoaded)
    {
        player.setPaused(false);
        //player.play();
    }
}
void PBModel::fadeOut()
{
    unsigned delay = 0;
    unsigned duration = 0;
    int target = 1;
    exitCount = 0;

    tweenexpo.setParameters(7,easingexpo,ofxTween::easeIn,target,0,duration,delay);
    if (bLoaded)
    {
        player.setPaused(true);
        //player.stop();
    }
}
void PBModel::drawModel()
{

//glActiveTexture(GL_TEXTURE0);
    glEnable(GL_DEPTH_TEST);
    glPushMatrix();

    glTranslatef(0,0,z);
    glRotatef(-90,0,0,1);
    for (int i = 0 ; i < numModels ; i ++)
    {
        models[i]->draw();
    }
    glPopMatrix();

    //ofSetupScreen();
    glPushMatrix();
    glTranslatef(-w/2,-h/2,0);
    if (bLoaded)
    {

        ofSetColor(255,255,255);

        //player.draw(0,0);

        shaderBGSub.setShaderActive(true);
        shaderBGSub.setUniform("tex1", 1);
        shaderBGSub.setUniform("tex2", 2);
        shaderBGSub.setUniform("tol", tol);
        //player.draw(-w/2,-h/2);
        glBegin(GL_QUADS);

        glTexCoord2f(0, 0);
        glVertex2f(vidPoint[0].x, vidPoint[0].y);

        glTexCoord2f(0, 1);
        glVertex2f(vidPoint[1].x, vidPoint[1].y);

        glTexCoord2f(1, 1);
        glVertex2f(vidPoint[2].x, vidPoint[2].y);

        glTexCoord2f(1, 0);
        glVertex2f(vidPoint[3].x, vidPoint[3].y);

        glEnd();

        myTexture2.unbind();
        myTexture.unbind();

        shaderBGSub.setShaderActive(false);
        glDisable(GL_DEPTH_TEST);


    }

    glPopMatrix();

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
