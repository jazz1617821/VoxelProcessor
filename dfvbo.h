#ifndef _DISTANCE_TRANSFORM_VBO_H
#define _DISTANCE_TRANSFORM_VBO_H

#include "vbo.h"
#include "distancefield.h"

typedef enum { TD_IN, TD_OUT, TD_BOTH }
THICK_DIRECTION;

VBO *createDistanceFieldVBO(DistanceField *df);

void createDFClipPlane(DistanceField *df);

void updateDFClipPlane(int clipIdx, DistanceField *df);

#endif
