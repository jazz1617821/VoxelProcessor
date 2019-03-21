#pragma once
#include "boundingbox.h"
#include <stdlib.h>

#define EDITSPACERESOLUTION 1024

#define VOX_EMPTY 0
#define VOX_SURFACE 1
#define VOX_SOLID 2
#define VOX_IN_THICK 3
#define VOX_OUT_THICK 4

#define VOX_NAME_SIZE 50

typedef struct EditVoxel
{
	unsigned char data;
	unsigned short ID;
}EditVoxel;

typedef struct EditVoxelData
{
	char name[VOX_NAME_SIZE];		//Data Name
	int resolution[3];				//Data resolution
	float voxelSize[3];				//Data voxle size
	EditVoxel *rawData;				//Raw data
	AABB *bbox;

	// render staff
	VBO *vbo;
}EditVoxelData;

EditVoxelData* newEditVoxelData(void);
void deleteEditVoxelData(EditVoxelData**);



