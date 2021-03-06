#include "CountDown.h"

CountDown::CountDown()
{
    //ctor
    mCount = -1;;
    countImage = NULL;
    num = 0;
    bShow = false;
}

CountDown::~CountDown()
{
    //dtor
}
void CountDown::setup(int _second)
{
    ofxDirList dir;
    dir.allowExt("png");
    num  = dir.listDir("images/counter");
    countImage = new ofImage[num];
    for (int i = 0 ; i < num ; i++)
    {
        countImage[i].loadImage(dir.getPath(i));
    }
    mTimer.setup(_second,true);
    stop();
    ofAddListener(mTimer.TIMER_REACHED,this,&CountDown::update);
    second = _second;
    shutter.loadSound("sounds/camera.wav");
    //gui.addPage("CountDown");
    gui.addSlider("scale",scale,0,2);

}
void CountDown::update(ofEventArgs &args)
{
    if (mCount==num-1)
    {
        static ofEventArgs timerEventArgs;
        ofNotifyEvent(COUNTER_REACHED, timerEventArgs, this);
    }
    if (mCount==num-1)
    {

        shutter.play();
        stop();
    }
    mCount++;
    mCount%=num;
}
void CountDown::draw()//bool blips)
{
    if (bShow)
    {
        ofPushStyle();
        ofEnableAlphaBlending();
        //for (int i = 0 ; i < num ; i++)
        if (mCount>-1)
        {
            int centerW = ofGetWidth()/2;
            int centerH = ofGetHeight()/2;
            ofImage *temp;
            /*if (mCount==num-2 && blips)
            {
                temp = &countImage[num-1];
            }
            else
            {*/
                temp = &countImage[mCount];
            //}
            float _w = temp->width*scale;
            float _h = temp->height*scale;
            int tW = _w/2;
            int tH = _h/2;
            int _x = centerW-tW;
            int _y = centerH-tH;

            temp->draw(_x,_y,_w,_h);

        }
        ofDisableAlphaBlending();
        ofPopStyle();
    }
}
void CountDown::start()
{
    mCount = 0;
    mTimer.startTimer();
    bShow = true;
}
void CountDown::stop()
{
    mTimer.stopTimer();
    bShow = false;
}
