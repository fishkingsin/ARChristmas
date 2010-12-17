#include "ModelHandler.h"

void ModelHandler::setup()
{

    numModels = models.size();
}

//--------------------------------------------------------------
void ModelHandler::update()
{

    for (int i = 0 ; i<numModels ; i++)
    {
        models[i]->update();

    }


}

//--------------------------------------------------------------
void ModelHandler::updateModelMatrix(int _i , float *m , float *project)
{
    for (int i = 0 ; i<numModels ; i++)
    {
        models[i]->setModelMatrix(_i,m , project);
    }

}

void ModelHandler::draw()
{
    for (int i = 0 ; i<numModels ; i++)
    {
        models[i]->draw();
    }
}


void ModelHandler::draw2D()
{
    for (int i = 0 ; i<numModels ; i++)
    {
        models[i]->draw2D();
    }
}

