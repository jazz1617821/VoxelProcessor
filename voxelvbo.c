#include "voxelvbo.h"
#include "mymath.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define X 0
#define Y 1
#define Z 2

extern float g_cppArray[NUM_CPP_DIR][4];

VBO *createVoxelVBO(VoxelData *vdata)
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
	vbo->attribs[FACE] = (float*)calloc(numFaces * 144, sizeof(float));
	boxVBO = newBox(NULL);
	boxWFVBO = newBoxWF(NULL);
	scale(vdata->voxelSize[X], vdata->voxelSize[Y], vdata->voxelSize[Z], scaleMat);
	vbo->numVertices = 0;
	vbo->numWFVertices = 0;
	vbo->numWFLoop = 4;

	toColorID(vdata->id, colorID);
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
				for (l = 0; l < boxVBO->numVertices; ++l)
				{
					memcpy(vbo->attribs[FACE] + vbo->numVertices * 4, colorID, 4 * sizeof(float));
				}
				vbo->numVertices += boxVBO->numVertices;
				vbo->numWFVertices += boxWFVBO->numVertices;
				count++;
			}
		}
	}
	deleteVBO(&boxVBO);
	deleteVBO(&boxWFVBO);


	// create aabbVBO
	boxVBO = newBox(NULL);
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
	translateVBO(boxVBO, false);

	vbo->enable = true;
	vbo->enableBuffers[VERTEX] =
		vbo->enableBuffers[NORMAL] =
		vbo->enableBuffers[WIREFRAME] =
		vbo->enableBuffers[FACE] = true;

	vdata->bbox->vbo = boxVBO;
	vdata->vbo = vbo;

	return vbo;
}

void createVoxelClipPlane(VoxelData *vdata)
{
	int i, numVoxels;
	VBO *vbo;

	for (i = 0; i < NUM_CPP_DIR; ++i)
	{
		if (vdata->vbo->clip[i] == NULL)
			vdata->vbo->clip[i] = newClipPlane(g_cppArray[i]);

		vbo = vdata->vbo->clip[i]->vbo;
		if (vbo != NULL) {
			deleteVBO(&vbo);
		}
		vbo = newVBO();
		vbo->enableBuffers[VERTEX] =
			vbo->enableBuffers[NORMAL] =
			vbo->enableBuffers[COLOR] = true;
		vbo->material->isEnable = false;

		if (i == CPP_X || i == CPP_INV_X) {
			numVoxels = vdata->resolution[Y] * vdata->resolution[Z];
		}
		else if (i == CPP_Y || i == CPP_INV_Y) {
			numVoxels = vdata->resolution[X] * vdata->resolution[Z];
		}
		else if (i == CPP_Z || i == CPP_INV_Z) {
			numVoxels = vdata->resolution[X] * vdata->resolution[Y];
		}

		// create vertex array
		if (vbo->attribs[VERTEX] != NULL) free(vbo->attribs[VERTEX]);
		if (vbo->attribs[NORMAL] != NULL) free(vbo->attribs[NORMAL]);
		if (vbo->attribs[COLOR] != NULL) free(vbo->attribs[COLOR]);
		vbo->attribs[VERTEX] = (float*)calloc(numVoxels * 18, sizeof(float));
		assert(vbo->attribs[VERTEX] != NULL);
		vbo->attribs[NORMAL] = (float*)calloc(numVoxels * 18, sizeof(float));
		assert(vbo->attribs[NORMAL] != NULL);
		vbo->attribs[COLOR] = (float*)calloc(numVoxels * 24, sizeof(float));
		assert(vbo->attribs[COLOR] != NULL);

		// set vbo
		vdata->vbo->clip[i]->vbo = vbo;
	}

	// initialize clip plane values
	vdata->clip[CPP_X] = 0;
	vdata->clip[CPP_INV_X] = vdata->resolution[X];
	vdata->clip[CPP_Y] = 0;
	vdata->clip[CPP_INV_Y] = vdata->resolution[Y];
	vdata->clip[CPP_Z] = 0;
	vdata->clip[CPP_INV_Z] = vdata->resolution[Z];
	vdata->vbo->clip[CPP_X]->plane[3] = vdata->bbox->max[X];
	vdata->vbo->clip[CPP_INV_X]->plane[3] = vdata->bbox->max[X];
	vdata->vbo->clip[CPP_Y]->plane[3] = vdata->bbox->max[Y];
	vdata->vbo->clip[CPP_INV_Y]->plane[3] = vdata->bbox->max[Y];
	vdata->vbo->clip[CPP_Z]->plane[3] = vdata->bbox->max[Z];
	vdata->vbo->clip[CPP_INV_Z]->plane[3] = vdata->bbox->max[Z];
}

void updateVoxelClipPlane(int clipIdx, VoxelData *vdata)
{
	bool isInverse;
	int i, j, k, numVoxels, index, planeIdx, outer, inner, outerOffset, innerOffset, clipOffset, triClip, triInner, triOuter;
	float v0[3], v1[3], v2[3], v3[3], tileV[18], tileN[18], tileC[24], color[4], clipSize, innerSize, outerSize;
	ClipPlane *cpp;
	VBO *vbo;

	numVoxels = 0;
	cpp = vdata->vbo->clip[clipIdx];
	vbo = cpp->vbo;
	planeIdx = vdata->clip[clipIdx];
	// x clip
	if (clipIdx == CPP_X || clipIdx == CPP_INV_X) {
		if (planeIdx <= 0 || planeIdx >= vdata->resolution[X] - 1) {
			vbo->numVertices = 0;
			return;
		}
		outer = vdata->resolution[Y];
		inner = vdata->resolution[Z];
		outerOffset = vdata->resolution[X];
		innerOffset = vdata->resolution[X] * vdata->resolution[Y];
		clipOffset = 1;
		clipSize = planeIdx * vdata->voxelSize[X];
		outerSize = vdata->voxelSize[Y];
		innerSize = vdata->voxelSize[Z];
		isInverse = (clipIdx == CPP_INV_X) ? true : false;
		triClip = X;
		triOuter = Y;
		triInner = Z;
	}
	// y clip
	else if (clipIdx == CPP_Y || clipIdx == CPP_INV_Y) {
		if (planeIdx <= 0 || planeIdx >= vdata->resolution[Y] - 1) {
			vbo->numVertices = 0;
			return;
		}
		outer = vdata->resolution[X];
		inner = vdata->resolution[Z];
		outerOffset = 1;
		innerOffset = vdata->resolution[X] * vdata->resolution[Y];
		clipOffset = vdata->resolution[X];
		clipSize = planeIdx * vdata->voxelSize[Y];
		outerSize = vdata->voxelSize[X];
		innerSize = vdata->voxelSize[Z];
		isInverse = (clipIdx == CPP_INV_Y) ? true : false;
		triClip = Y;
		triOuter = X;
		triInner = Z;
	}
	// z clip
	else if (clipIdx == CPP_Z || clipIdx == CPP_INV_Z) {
		if (planeIdx <= 0 || planeIdx >= vdata->resolution[Z] - 1) {
			vbo->numVertices = 0;
			return;
		}
		outer = vdata->resolution[X];
		inner = vdata->resolution[Y];
		outerOffset = 1;
		innerOffset = vdata->resolution[X];
		clipOffset = vdata->resolution[X] * vdata->resolution[Y];
		clipSize = planeIdx * vdata->voxelSize[Z];
		outerSize = vdata->voxelSize[X];
		innerSize = vdata->voxelSize[Y];
		isInverse = (clipIdx == CPP_INV_Z) ? true : false;
		triClip = Z;
		triOuter = X;
		triInner = Y;
	}
	// create polygons on the clipping plane
	for (i = 0; i < outer; ++i)
	{
		for (j = 0; j < inner; ++j)
		{
			index = planeIdx * clipOffset + j * innerOffset + i * outerOffset;
			if (vdata->rawData[index].data != VOX_EMPTY) {
				if (vdata->rawData[index].data == VOX_SURFACE) {
					color[0] = 0.6;
					color[1] = 0.1;
					color[2] = 0.1;
					color[3] = 1.0;
				}
				else if (vdata->rawData[index].data == VOX_SOLID) {
					color[0] = 0.1;
					color[1] = 0.5;
					color[2] = 0.7;
					color[3] = 1.0;
				}
				else {
					color[0] = 0.0;
					color[1] = 0.0;
					color[2] = 0.0;
					color[3] = 1.0;
				}
				copy3fv(vdata->bbox->min, v0);
				copy3fv(vdata->bbox->min, v1);
				copy3fv(vdata->bbox->min, v2);
				copy3fv(vdata->bbox->min, v3);
				v0[triClip] += clipSize;	v0[triOuter] += i * outerSize;				v0[triInner] += j * innerSize;
				v1[triClip] += clipSize;	v1[triOuter] += i * outerSize;				v1[triInner] += (j + 1) * innerSize;
				v2[triClip] += clipSize;	v2[triOuter] += (i + 1) * outerSize;		v2[triInner] += (j + 1) * innerSize;
				v3[triClip] += clipSize;	v3[triOuter] += (i + 1) * outerSize;		v3[triInner] += j * innerSize;
				createPolygonVBO(v0, v1, v2, v3, tileV, tileN);
				
				if (isInverse) {
					//for (k = 0; k < 18; ++k) tileN[k] *= -1;
				}
				for (k = 0; k < 24; k += 4)
				{
					memcpy(vbo->attribs[COLOR] + numVoxels * 24 + k, color, 4 * sizeof(float));
				}
				memcpy(vbo->attribs[VERTEX] + numVoxels * 18, tileV, 18 * sizeof(float));
				memcpy(vbo->attribs[NORMAL] + numVoxels * 18, tileN, 18 * sizeof(float));
				numVoxels++;
			}
		} // end inner loop
	} // end outer loop
	vbo->numVertices = numVoxels * 6;
}

void setVoxelDataID(VoxelData *vdata, VBO *vbo)
{
	int i, j, k;
	float colorID[4];

	if (vbo->attribs[FACE] == NULL) {
		vbo->attribs[FACE] = (float*)calloc(vbo->numVertices * vbo->dataSize[FACE], sizeof(float));
	}

	toColorID(vdata->id, colorID);
	for (i = 0; i < vbo->numVertices; i += 4)
	{
		memcpy(vbo->attribs[FACE] + i, colorID, 4 * sizeof(float));
	}
}