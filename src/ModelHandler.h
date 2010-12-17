#define MODEL_HANDLER_H
#ifdef MODEL_HANDLER_H
#include "ARModelCollections.h"


#include "global.h"
class ModelHandler
{

public:

    void setup();
    void update();
    void draw();
    void draw2D();
    void updateModelMatrix(int i , float *m , float *project);
    vector <ARModelCollections*> getModel()
    {
        return models;
    }
    ARModelCollections* getActiveModel()
    {
        return activeModelIndex>-1? models[activeModelIndex] : NULL;
    }
    int getActiveModelIndex()
    {
        return activeModelIndex;
    }
    void addModelCollection(ARModelCollections *model)
    {
        models.push_back(model);
        numModels = models.size();
    }




private :
    int activeModelIndex;
    int numModels;
    float scale;

    vector <ARModelCollections*> models;
    ofxTween tweenlinear;
    ofxEasingLinear 	easinglinear;


};

#endif // MODEL_HANDLER_H
