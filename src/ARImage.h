#ifndef ARIMAGE_H
#define ARIMAGE_H

#include "global.h"

class ARImage : public ofxMSAInteractiveObject ,public ofImage
{
public:
    ARImage();
    void setup();
    void update();
    void draw();
    void draw(int _x, int _y);

protected:
private:
    ofxPerlin *perlin;
    ofxVec3f noise;
    float ballSize;

    float bsSeed;



};

#endif // ARIMAGE_H
