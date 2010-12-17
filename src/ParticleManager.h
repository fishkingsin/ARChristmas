#ifndef PARTICLEMANAGER_H
#define PARTICLEMANAGER_H

#include "ofMain.h"
#include "ofxVectorMath.h"
#include "ofxSimpleGuiToo.h"
#include "ofxDirList.h"
class Particle
{
public:
    Particle()
    {
        dead = false;
    }
    void update()
    {
        pos+=vec;
        age--;
        if (age<0)
        {
            dead = true;
        }
    }
    ofxVec2f pos;
    ofxVec2f vec;
    float size;
    int age;
    bool dead;

};
class ParticleManager
{
public:
    ParticleManager();
    virtual ~ParticleManager();
    void setup(string fn);
    void update();
    void draw(int i);
    void addParticle();
protected:
private:
    vector <Particle> particles;
    vector <ofImage> img;
    float x,y,maxsize,minsize,maxa,mina,count;
};

#endif // PARTICLEMANAGER_H
