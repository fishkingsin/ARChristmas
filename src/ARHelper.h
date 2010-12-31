/*
	ARChristmas is a app implemented ARToolkitplus 
    Copyright (C) <2010>  <James Kong>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#define OF_ADDON_USING_OFXOPENCV
#define OF_ADDON_USING_OFXOBJLOADER
#define OF_ADDON_USING_OFXDIRLIST
#define OF_ADDON_USING_OFXVECTORMATH
#define OF_ADDON_USING_OFXNETWORK
#define OF_ADDON_USING_OFXVECTORGRAPHICS
#define OF_ADDON_USING_OFXOSC
#define OF_ADDON_USING_OFXTHREAD
#define OF_ADDON_USING_OFXXMLSETTINGS

#include "ofMain.h"
#include "ofAddons.h"
#include "ModelHandler.h"
#include "global.h"


class ARHelper
{
public:
    ARHelper();
    bool setup();
    void update(unsigned char *pixels ,int _width ,int _height);
    void draw();
    void videoSettings();
    void toggleMirror();
    void toggleDebug();
    void debugDraw(int id , float *m);
    bool isDetected()
    {
        return markerId>-1?true:false;
    }
    int  getMarkerId()
    {
        return markerId;
    }

    void setModelHandler(ModelHandler &mh)
    {
        modelHandler = &mh;
    }
    void setModelHandler(ModelHandler *mh)
    {
        modelHandler = mh;
    }
    ModelHandler *modelHandler;

protected:
private:
    ofTrueTypeFont myFont;
    //ofVideoGrabber grabber;
    ofxCvColorImage convert;
    ofxCvGrayscaleImage gray;

    int width, height, bpp,markerId,displayW,displayH;
    float conf;
    size_t        numPixels;
    size_t        numBytesRead;
    unsigned char *cameraBuffer;

    bool bDraw;
    bool useBCH;
    bool useCustom;
    bool bMirror;
    bool bDebug;


    ofxVec2f markerV;
    int tol;


    ofImage north;



};

