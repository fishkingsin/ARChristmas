#include "ARImage.h"
static float nX = 0;
static float nY = 0;
static float nZ = 0;
ARImage::ARImage()
{
    ofxMSAInteractiveObject::ofxMSAInteractiveObject();
    ofImage::ofImage();
    //cto
    //ofxMSAInteractiveObject::disableAllEvents();
    perlin = new ofxPerlin();
    ballSize = 10;
    bsSeed = ofRandomf();
}
void ARImage::setup()
{
    ofxMSAInteractiveObject::enableAllEvents();
    //ofxMSAInteractiveObject::disableAllEvents();
    //ofxMSAInteractiveObject::disableAppEvents();
    enabled = true;

}
void ARImage::update()
{
    noise.x=perlin->noise(nX+=0.00003f);
    noise.y=perlin->noise(nY+=0.000014);
    noise.z=perlin->noise(nZ+=0.000015f);
    noise*=100;
    ballSize = 30+sinf(bsSeed+=0.03f)*30/2;

}
void ARImage::draw()
{
    //ofImage::draw(x,y);
}
void ARImage::draw(int _x, int _y)
{

    float this_x = x;
    float this_y = y;
    float yD = abs((float)(this_y+_y)-ofGetHeight()/2);
    float alpha =  0;
    if(yD<IMAGE_AREA) alpha = sinf((IMAGE_AREA-yD)/IMAGE_AREA);

    float miw = getWidth();
    float mih = getHeight();

    float wR = 100*(1-(miw/ofGetWidth()));
    float hR = 100*(1-(mih/ofGetHeight()));

    float x1 = this_x+noise.x+_x-wR;
    float y1 = this_y+noise.y+_y-hR;
    float x2 = this_x+noise.x+(_x-wR)+50;
    float y2 = this_y+noise.y+_y-hR;

    glPushMatrix();
    //glScalef(1+(3*alpha),1+(3*alpha),1+(3*alpha));
    glTranslatef(noise.x,noise.y,0);//,noise.z);
    ofSetColor(255,255,255,255*alpha);
    ofImage::draw(this_x+_x-miw-wR,this_y+_y-hR-mih/2.0f);//,miw*(1+(alpha*2)),mih*(1+(alpha*2)));
    glPopMatrix();


    ofSetLineWidth(5);
    ofSetColor(71,66,67,255*alpha);
    ofLine(x1, y1, x2,y2);
    ofLine(x2, y2, this_x+_x,this_y+_y);

//ofPushStyle();
//ofEnableAlphaBlending();
    ofNoFill();
    ofSetLineWidth(10);
    ofSetColor(255,0,0,125);
    ofCircle(this_x+_x,this_y+_y,ballSize);
//    ofDisableAlphaBlending();
 //   ofPopStyle();
    #ifdef    DEBUG
    ofSetColor(255,0,0);
    ofRect(this_x+_x,this_y+_y,50,50);
#endif
}

