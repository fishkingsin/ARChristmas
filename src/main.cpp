#include "ofMain.h"
#include "testApp.h"
#include <stdio.h>
#include <stdlib.h>

int main( )
{
    //ofAppGlutWindow window;
    //window.setGlutDisplayString("rgba double samples>=4 depth");
    //ofSetupOpenGL(&window, 640,480, OF_GAME_MODE);			// <-------- setup the GL context
    ofSetupOpenGL( 1280,720, OF_WINDOW);			// <-------- setup the GL context

    // this kicks off the running of my app
    // can be OF_WINDOW or OF_FULLSCREEN
    // pass in width and height too:
    ofRunApp(new testApp());


}
