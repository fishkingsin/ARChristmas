#include "ParticleManager.h"

ParticleManager::ParticleManager()
{
    //ctor
}

ParticleManager::~ParticleManager()
{
    //dtor
}
void ParticleManager::setup(string fn)
{
    ofxDirList dir;
    dir.allowExt("png");

    int num = dir.listDir("images/particles");
    for (int i = 0 ; i < num ; i++)
    {
        ofImage temp;
        temp.loadImage(dir.getPath(i));
        img.push_back(temp);
    }
    gui.addPage("ParticleManager");
    gui.addSlider("count",count,10,50);
    gui.addSlider("x",x,0,2000);
    gui.addSlider("y",y,0,1000);
    gui.addSlider("max size",maxsize,1,256);
    gui.addSlider("min size",minsize,1,256);
    gui.addSlider("max a",maxa,-10,0);
    gui.addSlider("min a",mina,0,10);
}
void ParticleManager::update()
{


}
void ParticleManager::draw(int i)
{
    if (i<0 || i>img.size())
        return;
    ofEnableAlphaBlending();
    vector <Particle>::iterator iter = particles.begin();
    while (iter != particles.end())
    {
        ofSetColor(255,255,255);
        img[i-1].draw(iter->pos.x-iter->size/2,iter->pos.y-iter->size/2,iter->size,iter->size);
        iter->update();
        if (iter->dead)
        {

            iter = particles.erase(iter);
        }
        else ++iter;
    }
    ofDisableAlphaBlending();
}
void ParticleManager::addParticle()
{
    for (int i = 0 ; i < count ; i++)
    {
        Particle p;
        p.pos.set(x,y);
        p.vec.set(ofRandom(maxa,mina),ofRandom(maxa,mina));
        p.size = ofRandom(minsize,maxsize);
        p.age = ofRandom(50,100);
        particles.push_back(p);
    }
}
