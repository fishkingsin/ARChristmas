#ifndef ARMODEL_H
#define ARMODEL_H

#include "ofx3DModelLoader.h"
#include "ofxVectorMath.h"
#include "global.h"
#include "ARImage.h"

class ARModel : public ofx3DModelLoader
{
    public:
    void setup();
//    void setupPoints(int numPoints ,string name);
    void addImage(ARImage *img);

    void draw();
    void setARModelPosition3D(ofxVec3f v3);
    void setARModelPosition2D(ofxVec2f v2);
    ofxVec3f *getPos3d();
    ofxVec2f *getPos2d();
    vector <ofPoint> points2d;
    void modelVecTranslate(ofxVec3f vec);
    protected:
    ofxVec3f pos3d;
    ofxVec2f pos2d;

    private:
    vector <ARImage*> images;
};

#endif // ARMODEL_H
