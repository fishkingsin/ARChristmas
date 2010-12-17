#include "ARModel.h"
void ARModel::setup()
{
    setRotation(0, 90, 1, 0, 0);
    //setRotation(0, -90, 0,0, 1);

    setPosition(ofGetWidth()/2, ofGetHeight()/2, 0);


}
//void ARModel::setupPoints(int numPoints ,string name)
//{
//
//
//    for(int i = 0 ; i< numPoints ; i++)
//    {
//        ofPoint p;
//        points2d.push_back(p);
//
//    }
//    for(int i = 0 ; i< points2d.size() ; i++)
//    {
//        gui.addSlider2d(name+"Point"+ofToString(i),points2d[i],-60,60,-60,60);
//    }
//
//}
void ARModel::addImage(ARImage *img)
{
    if (img!=NULL)
    {
        images.push_back(img);
        ofPoint p;
        points2d.push_back(p);
        //gui.addSlider2d("Point"+ofToString((int)points2d.size()),points2d[points2d.size()],-60,60,-60,60);
    }
}
void ARModel::draw()
{
    glPushMatrix();
    modelVecTranslate(pos3d);
    glPushMatrix();
    glTranslatef(-ofGetWidth()/2.0f,-ofGetHeight()/2.0f,0);



    ofx3DModelLoader::draw();
    glPopMatrix();
    ofEnableAlphaBlending();
    for (int p = 0; p<points2d.size() ; p++)
    {
        glPushMatrix();
        glTranslatef(-images[p]->getWidth()/2,-images[p]->getHeight()/2,0);
        glRotatef(-90,1,0,0);
        glTranslatef(points2d[p].x,points2d[p].y,0);

        images[p]->draw(0,0);
        //ofCircle(points2d[p].x,points2d[p].y,5);
        //ofDrawBitmapString("Text"+ofToString(p),points2d[p].x,points2d[p].y);
        glPopMatrix();
    }
    ofDisableAlphaBlending();
    glPopMatrix();


}
void ARModel::setARModelPosition3D(ofxVec3f v3)
{
    pos3d = v3;

}
void ARModel::setARModelPosition2D(ofxVec2f v2)
{
    pos2d = v2;
}
ofxVec3f* ARModel::getPos3d()
{
    return &pos3d;
}
ofxVec2f* ARModel::getPos2d()
{

    return &pos2d;
}
void ARModel::modelVecTranslate(ofxVec3f v3)
{
    glTranslatef(v3.x,v3.y,v3.z);
}
