#ifndef PBMODEL_H
#define PBMODEL_H

#include "ARModelCollections.h"

class PBModel : public ARModelCollections
{
public:
    PBModel(int _id,float _scale, string _folder);
    virtual ~PBModel();
    void selfUpdate();
    void selfSetup();
    void drawModel();
    void draw2D();
    void findScreenCoord( const GLdouble *proj, const GLint *view);
    void fadeIn();
    void fadeOut();

protected:
private:


    int z;
    ofPoint *points;
    ofPoint *modelPnts;
    int alpha;
    bool debug;


};

#endif // PBModel_H
