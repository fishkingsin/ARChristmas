#include "ARModelCollections.h"
#include "ofVideoPlayerGst.h"
#include "shaderChromaKey.h"
#include "global.h"
#define USE_GSTREAMER
class MiddleVillage :public ARModelCollections
{

public:
    MiddleVillage()
    {
        ARModelCollections::ARModelCollections();
        id = 2;
        scale =1;//0.1;

        folder = "MiddleVillage";
        model_path = model_path+folder;
        image_path = image_path+folder;



    }
    void selfSetup()
    {
        //sndPlayer.loadSound(sound_path+"OMAinfoMUSICAug1(withoutdru.wav");
        sndPlayer.loadSound(sound_path+"middlevillage.mp3");
        secondImage.loadImage("images/green.jpg");
        secondImage.resize(ofGetWidth(),ofGetHeight());

        shaderBGSub.loadShader("shaders/mix");


        bLoaded = player.loadMovie("movies/middlevillage.mp4");

        if (bLoaded)
        {
            ofDisableArbTex();
            player.play();
            player.setLoopState(OF_LOOP_NORMAL);
            player.setPaused(true);



            w = player.getWidth();
            h = player.getHeight();


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
            ofLog(OF_LOG_FATAL_ERROR,"Can not Load model movie!");
        }
        gui.addPage("Middle Village Images");
        points = new ofPoint[numModelImages];

        gui.addSlider("pointZ",pointZ,-100,100);
        for (int i = 0 ; i < numModelImages ; i ++)
        {
            gui.addSlider2d("Point "+ofToString(i),points[i],-200,200,-200,200);
        }

    }

    void fadeIn()
    {
        unsigned duration = 5000;
        unsigned delay = 5;
        int target = 1;

        tweenexpo.setParameters(7,easingexpo,ofxTween::easeOut,0,target,duration,delay);
        player.setPaused(false);
    }
    void fadeOut()
    {
        unsigned delay = 0;
        unsigned duration = 0;
        int target = 1;
        exitCount = 0;

        tweenexpo.setParameters(7,easingexpo,ofxTween::easeIn,target,0,duration,delay);
        player.setPaused(true);
    }
    void selfUpdate()
    {

        if (bLoaded)
        {
            player.update();

            unsigned char *imgPixels = player.getPixels();
            myTexture.loadData(imgPixels, w,h, GL_RGB);
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, myTexture.getTextureData().textureID);


            // go back to default texture unit !!
            glActiveTexture(GL_TEXTURE0);
        }
        for (int i = 0 ; i < numModels ; i ++)
        {
            models[i]->setARModelPosition3D(ofxVec3f(0,0,modelZ+tweenScale*scale*i*(tweenexpo.update()/numModels)));
        }
    }
    void draw2D()
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
    void drawModel()
    {


        glEnable(GL_DEPTH_TEST);
        glPushMatrix();
        glTranslatef(0,0,z);

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

    void findScreenCoord( const GLdouble *proj, const GLint *view)
    {
        GLdouble ret_x, ret_y, ret_z;
        float x = 0;
        float y = 0;
        for (int i = 0 ; i < numModelImages ; i ++)
        {



            gluProject( models[0]->getPos3d()->x + points[i].x,
                        models[0]->getPos3d()->y + points[i].y,
                        models[0]->getPos3d()->z + pointZ,
                        targetMatrix,//get the modle matrix from modelset to calc each model screen coord
                        proj,
                        view, &ret_x, &ret_y, &ret_z);
            x = ret_x;
            y = ofGetHeight()-ret_y;
            //models[i]->setARModelPosition2D(ofxVec2f(x,y));
            model_images[i]->setPos(x,y);
        }
    }


private:
#ifdef USE_GSTREAMER
    ofVideoPlayerGst player;
#else
    ofVideoPlayer player;
#endif
    bool bLoaded;
    int w,h;

    ofxShader shaderBGSub;

    ofTexture myTexture,myTexture2;

    ofImage secondImage;
    ofPoint vidPoint[4];
    float tol;
    int z;
    ofPoint *points;
    float pointZ;


};
