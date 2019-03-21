#include "editvoxelvbo.h"
#include "mymath.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define X 0
#define Y 1
#define Z 2

VBO *createEditVoxelVBO(EditVoxelData* vdata)
{
	bool neighbor[26];
	int i, j, k, l, m, values[3], index, count, numFaces, resolution[3];
	float transMat[16], scaleMat[16], ids[144], colorID[4], origin[3], center[3];
	VBO* vbo;
	VBO* boxVBO;
	VBO* boxWFVBO;

	origin[X] = vdata->bbox->min[X];
	origin[Y] = vdata->bbox->min[Y];
	origin[Z] = vdata->bbox->min[Z];

	// count drawing faces
	numFaces = 0;
	for (i = 0; i < vdata->resolution[Z]; ++i)
	{
		for (j = 0; j < vdata->resolution[Y]; ++j)
		{
			for (k = 0; k < vdata->resolution[X]; ++k)
			{
				index = k + j * vdata->resolution[X] + i * vdata->resolution[X] * vdata->resolution[Y];
				if (vdata->rawData[index].data == VOX_EMPTY) continue;
				(k + 1 < vdata->resolution[X] && vdata->rawData[index + 1].data) ?
					0 : numFaces++;
				(k > 1 && vdata->rawData[index - 1].data) ?
					0 : numFaces++;
				(j + 1 < vdata->resolution[Y] && vdata->rawData[index + vdata->resolution[X]].data) ?
					0 : numFaces++;
				(j > 1 && vdata->rawData[index - vdata->resolution[X]].data) ?
					0 : numFaces++;
				(i + 1 < vdata->resolution[Z] && vdata->rawData[index + vdata->resolution[X] * vdata->resolution[Y]].data) ?
					0 : numFaces++;
				(i > 1 && vdata->rawData[index - vdata->resolution[X] * vdata->resolution[Y]].data) ?
					0 : numFaces++;
			}
		}
	}


	// creat vbo
	vbo = newVBO();
	strcpy(vbo->name, vdata->name);
	vbo->attribs[VERTEX] = (float*)calloc(numFaces * 108, sizeof(float));
	vbo->attribs[NORMAL] = (float*)calloc(numFaces * 108, sizeof(float));
	vbo->attribs[WIREFRAME] = (float*)calloc(numFaces * 108, sizeof(float));
	//vbo->attribs[FACE] = (float*)calloc(numFaces * 144, sizeof(float));
	boxVBO = newBox(NULL);
	boxWFVBO = newBoxWF(NULL);
	scale(vdata->voxelSize[X], vdata->voxelSize[Y], vdata->voxelSize[Z], scaleMat);
	vbo->numVertices = 0;
	vbo->numWFVertices = 0;
	vbo->numWFLoop = 4;

	//toColorID(vdata->id, colorID);
	count = 0;
	for (i = 0; i < vdata->resolution[Z]; ++i)
	{
		for (j = 0; j < vdata->resolution[Y]; ++j)
		{
			for (k = 0; k < vdata->resolution[X]; ++k)
			{
				index = k + j * vdata->resolution[X] + i * vdata->resolution[X] * vdata->resolution[Y];
				if (vdata->rawData[index].data == VOX_EMPTY) continue;
				neighbor[0] = (k + 1 < vdata->resolution[X] && vdata->rawData[index + 1].data) ?
					false : true;
				neighbor[1] = (k > 1 && vdata->rawData[index - 1].data) ?
					false : true;
				neighbor[2] = (j + 1 < vdata->resolution[Y] && vdata->rawData[index + vdata->resolution[X]].data) ?
					false : true;
				neighbor[3] = (j > 1 && vdata->rawData[index - vdata->resolution[X]].data) ?
					false : true;
				neighbor[4] = (i + 1 < vdata->resolution[Z] && vdata->rawData[index + vdata->resolution[X] * vdata->resolution[Y]].data) ?
					false : true;
				neighbor[5] = (i > 1 && vdata->rawData[index - vdata->resolution[X] * vdata->resolution[Y]].data) ?
					false : true;
				initBox(boxVBO, neighbor);
				initBoxWF(boxWFVBO, neighbor);
				translate(origin[0] + k * vdata->voxelSize[X],
					origin[1] + j * vdata->voxelSize[Y],
					origin[2] + i * vdata->voxelSize[Z],
					transMat);
				multMat4fv(transMat, scaleMat, boxVBO->modelMat);
				multMat4fv(transMat, scaleMat, boxWFVBO->modelMat);
				translateVBO(boxVBO, false);
				translateVBO(boxWFVBO, false);
				memcpy_s(vbo->attribs[VERTEX] + vbo->numVertices * 3, boxVBO->numVertices * 3 * sizeof(float), boxVBO->attribs[VERTEX], boxVBO->numVertices * 3 * sizeof(float));
				memcpy_s(vbo->attribs[NORMAL] + vbo->numVertices * 3, boxVBO->numVertices * 3 * sizeof(float), boxVBO->attribs[NORMAL], boxVBO->numVertices * 3 * sizeof(float));
				memcpy_s(vbo->attribs[WIREFRAME] + vbo->numWFVertices * 3, boxWFVBO->numVertices * 3 * sizeof(float), boxWFVBO->attribs[VERTEX], boxWFVBO->numVertices * 3 * sizeof(float));
				/*for (l = 0; l < boxVBO->numVertices; ++l)
				{
					memcpy(vbo->attribs[FACE] + vbo->numVertices * 4, colorID, 4 * sizeof(float));
				}*/
				vbo->numVertices += boxVBO->numVertices;
				vbo->numWFVertices += boxWFVBO->numVertices;
				count++;
			}
		}
	}
	deleteVBO(&boxVBO);
	deleteVBO(&boxWFVBO);


	// create aabbVBO
	/*boxVBO = newBox(NULL);
	boxVBO->enableBuffers[FACE] = true;
	boxVBO->attribs[FACE] = (float*)calloc(144, sizeof(float));
	toColorID(vdata->id, colorID);
	for (i = 0; i < 144; i += 4)
	{
		boxVBO->attribs[FACE][i] = colorID[0];
		boxVBO->attribs[FACE][i + 1] = colorID[1];
		boxVBO->attribs[FACE][i + 2] = colorID[2];
		boxVBO->attribs[FACE][i + 3] = colorID[3];
	}

	aabbCenter(*vdata->bbox, center);
	translate(center[X], center[Y], center[Z], transMat);
	scale(fabs(vdata->bbox->max[X] - vdata->bbox->min[X]),
		fabs(vdata->bbox->max[Y] - vdata->bbox->min[Y]),
		fabs(vdata->bbox->max[Z] - vdata->bbox->min[Z]),
		scaleMat);
	multMat4fv(scaleMat, boxVBO->modelMat, boxVBO->modelMat);
	multMat4fv(transMat, boxVBO->modelMat, boxVBO->modelMat);
	translateVBO(boxVBO, false);*/

	vbo->enable = true;
	vbo->enableBuffers[VERTEX] =
		vbo->enableBuffers[NORMAL] =
		vbo->enableBuffers[WIREFRAME] = true;
		//vbo->enableBuffers[FACE] = true;

	//vdata->bbox->vbo = boxVBO;
	vdata->vbo = vbo;

	return vbo;
}