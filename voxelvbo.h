#ifndef _VOXELVBO_H
#define _VOXELVBO_H

#include "voxel.h"
#include "vbo.h"

VBO *createVoxelVBO(VoxelData *vdata);
void createVoxelClipPlane(VoxelData *vdata);
void updateVoxelClipPlane(int clipIdx, VoxelData *vdata);
void setVoxelDataID(VoxelData *vdata, VBO *vbo);

#endif // _VOXELVBO_H
