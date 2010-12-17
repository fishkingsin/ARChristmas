#ifndef COUNTDOWN_H
#define COUNTDOWN_H
#include "ofMain.h"
#include "ofxTimer.h"
#include "ofxDirList.h"
#include "ofxTween.h"
#include "ofxSimpleGuiToo.h"
class CountDown
{
    public:
    ofEvent <ofEventArgs> COUNTER_REACHED;
        CountDown();
        virtual ~CountDown();
        void setup(int _second);
        void update(ofEventArgs &args);
        void draw();//bool blips);
        void start();
        void stop();

    protected:
    private:

    ofxTimer mTimer;
    int mCount;
    ofImage *countImage;
    int num ;
    bool bShow;
    float second;
    ofSoundPlayer  shutter;
    float scale;

};

#endif // COUNTDOWN_H
