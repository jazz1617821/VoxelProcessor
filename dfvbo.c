#include "dfvbo.h"
#include "mymath.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define X 0
#define Y 1
#define Z 2

extern float g_cppArray[NUM_CPP_DIR][4];

VBO *createDistanceFieldVBO(DistanceField *df)
{
	bool neighbor[26];
	int i, j, k, l, m, values[3], index, count, numFaces, resolution[3];
	THICK_DIRECTION tdType;
	float translateMat[16], scaleMat[16], ids[144], colorID[4], origin[3];
	VBO *vbo;
	VBO *boxVBO;
	VBO *boxWFVBO;

	// df origin
	origin[X] = df->bbox->min[X];
	origin[Y] = df->bbox->min[Y];
	origin[Z] = df->bbox->min[Z];

	// count drawing faces
	numFaces = 0;
	for (i = 0; i < df->resolution[Z]; ++i)
	{
		for (j = 0; j < df->resolution[Y]; ++j)
		{
			for (k = 0; k < df->resolution[X]; ++k)
			{
				index = k + j * df->resolution[X] + i * df->resolution[X] * df->resolution[Y];
				if (df->data[index] < df->innerThick || df->data[index] > df->outerThick) {
					continue;
				}
				(k + 1 < df->resolution[X] &&
				df->data[index + 1] >= df->innerThick &&
				df->data[index + 1] <= df->outerThick) ?
					0 : numFaces++;
				(k > 1 &&
				df->data[index - 1] >= df->innerThick &&
				df->data[index - 1] <= df->outerThick) ?
					0 : numFaces++;
				(j + 1 < df->resolution[Y] &&
				df->data[index + df->resolution[X]] >= df->innerThick &&
				df->data[index + df->resolution[X]] <= df->outerThick) ?
					0 : numFaces++;
				(j > 1 &&
				df->data[index - df->resolution[X]] >= df->innerThick &&
				df->data[index - df->resolution[X]] <= df->outerThick) ?
					0 : numFaces++;
				(i + 1 < df->resolution[Z] &&
				df->data[index + df->resolution[X] * df->resolution[Y]] >= df->innerThick &&
				df->data[index + df->resolution[X] * df->resolution[Y]] <= df->outerThick) ?
					0 : numFaces++;
				(i > 1 &&
				df->data[index - df->resolution[X] * df->resolution[Y]] >= df->innerThick &&
				df->data[index - df->resolution[X] * df->resolution[Y]] <= df->outerThick) ?
					0 : numFaces++;
			}
		}
	}


	// creat vbo
	vbo = newVBO();
	strcpy(vbo->name, "DF");
	vbo->attribs[VERTEX] = (float*)calloc(numFaces * 108, sizeof(float));
	vbo->attribs[NORMAL] = (float*)calloc(numFaces * 108, sizeof(float));
	vbo->attribs[WIREFRAME] = (float*)calloc(numFaces * 108, sizeof(float));
	vbo->attribs[FACE] = (float*)calloc(numFaces * 144, sizeof(float));
	boxVBO = newBox(NULL);
	boxWFVBO = newBoxWF(NULL);
	scale(df->voxelSize[X], df->voxelSize[Y], df->voxelSize[Z], scaleMat);
	vbo->numVertices = 0;
	vbo->numWFLoop = 4;

	toColorID(df->id, colorID);
	count = 0;
	for (i = 0; i < df->resolution[Z]; ++i)
	{
		for (j = 0; j < df->resolution[Y]; ++j)
		{
			for (k = 0; k < df->resolution[X]; ++k)
			{
				index = k + j * df->resolution[X] + i * df->resolution[X] * df->resolution[Y];
				if (df->data[index] < df->innerThick || df->data[index] > df->outerThick) {
					continue;
				}
				neighbor[0] = 
					(k + 1 < df->resolution[X] &&
					df->data[index + 1] >= df->innerThick &&
					df->data[index + 1] <= df->outerThick) ?
					false : true;
				neighbor[1] =
				(k > 1 &&
					df->data[index - 1] >= df->innerThick &&
					df->data[index - 1] <= df->outerThick) ?
					false : true;
				neighbor[2] =
				(j + 1 < df->resolution[Y] &&
					df->data[index + df->resolution[X]] >= df->innerThick &&
					df->data[index + df->resolution[X]] <= df->outerThick) ?
					false : true;
				neighbor[3] =
				(j > 1 &&
					df->data[index - df->resolution[X]] >= df->innerThick &&
					df->data[index - df->resolution[X]] <= df->outerThick) ?
					false : true;
				neighbor[4] =
				(i + 1 < df->resolution[Z] &&
					df->data[index + df->resolution[X] * df->resolution[Y]] >= df->innerThick &&
					df->data[index + df->resolution[X] * df->resolution[Y]] <= df->outerThick) ?
					false : true;
				neighbor[5] =
				(i > 1 &&
					df->data[index - df->resolution[X] * df->resolution[Y]] >= df->innerThick &&
					df->data[index - df->resolution[X] * df->resolution[Y]] <= df->outerThick) ?
					false : true;
				initBox(boxVBO, neighbor);
				initBoxWF(boxWFVBO, neighbor);
				translate(origin[0] + k * df->voxelSize[X],
					origin[1] + j * df->voxelSize[Y],
					origin[2] + i * df->voxelSize[Z],
					translateMat);
				multMat4fv(translateMat, scaleMat, boxVBO->modelMat);
				multMat4fv(translateMat, scaleMat, boxWFVBO->modelMat);
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
	vbo->enable = true;
	vbo->enableBuffers[VERTEX] =
		vbo->enableBuffers[NORMAL] =
		vbo->enableBuffers[WIREFRAME] =
		vbo->enableBuffers[FACE] = true;
	deleteVBO(&boxVBO);
	deleteVBO(&boxWFVBO);

	df->vbo = vbo;

	return vbo;
}

void createDFClipPlane(DistanceField *df)
{
	int i, numVoxels;
	VBO *vbo;

	for (i = 0; i < NUM_CPP_DIR; ++i)
	{
		if (df->vbo->clip[i] == NULL)
			df->vbo->clip[i] = newClipPlane(g_cppArray[i]);

		vbo = df->vbo->clip[i]->vbo;
		if (vbo != NULL) {
			deleteVBO(&vbo);
		}
		vbo = newVBO();
		vbo->enableBuffers[VERTEX] =
			vbo->enableBuffers[NORMAL] =
			vbo->enableBuffers[COLOR] = true;
		vbo->material->isEnable = false;

		if (i == CPP_X || i == CPP_INV_X) {
			numVoxels = df->resolution[Y] * df->resolution[Z];
		}
		else if (i == CPP_Y || i == CPP_INV_Y) {
			numVoxels = df->resolution[X] * df->resolution[Z];
		}
		else if (i == CPP_Z || i == CPP_INV_Z) {
			numVoxels = df->resolution[X] * df->resolution[Y];
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
		df->vbo->clip[i]->vbo = vbo;
	}

	// initialize clip plane values
	df->clip[CPP_X] = 0;
	df->clip[CPP_INV_X] = df->resolution[X];
	df->clip[CPP_Y] = 0;
	df->clip[CPP_INV_Y] = df->resolution[Y];
	df->clip[CPP_Z] = 0;
	df->clip[CPP_INV_Z] = df->resolution[Z];
	df->vbo->clip[CPP_X]->plane[3] = df->bbox->max[X];
	df->vbo->clip[CPP_INV_X]->plane[3] = df->bbox->max[X];
	df->vbo->clip[CPP_Y]->plane[3] = df->bbox->max[Y];
	df->vbo->clip[CPP_INV_Y]->plane[3] = df->bbox->max[Y];
	df->vbo->clip[CPP_Z]->plane[3] = df->bbox->max[Z];
	df->vbo->clip[CPP_INV_Z]->plane[3] = df->bbox->max[Z];
}

void updateDFClipPlane(int clipIdx, DistanceField *df)
{
	bool isInverse;
	int i, j, k, numVoxels, index, planeIdx, outer, inner, outerOffset, innerOffset, clipOffset, triClip, triInner, triOuter;
	float v0[3], v1[3], v2[3], v3[3], tileV[18], tileN[18], tileC[24], color[4], clipSize, innerSize, outerSize;
	ClipPlane *cpp;
	VBO *vbo;

	numVoxels = 0;
	cpp = df->vbo->clip[clipIdx];
	vbo = cpp->vbo;
	planeIdx = df->clip[clipIdx];
	// x clip
	if (clipIdx == CPP_X || clipIdx == CPP_INV_X) {
		if (planeIdx <= 0 || planeIdx >= df->resolution[X] - 1) {
			vbo->numVertices = 0;
			return;
		}
		outer = df->resolution[Y];
		inner = df->resolution[Z];
		outerOffset = df->resolution[X];
		innerOffset = df->resolution[X] * df->resolution[Y];
		clipOffset = 1;
		clipSize = planeIdx * df->voxelSize[X];
		outerSize = df->voxelSize[Y];
		innerSize = df->voxelSize[Z];
		isInverse = (clipIdx == CPP_INV_X) ? true : false;
		triClip = X;
		triOuter = Y;
		triInner = Z;
	}
	// y clip
	else if (clipIdx == CPP_Y || clipIdx == CPP_INV_Y) {
		if (planeIdx <= 0 || planeIdx >= df->resolution[Y] - 1) {
			vbo->numVertices = 0;
			return;
		}
		outer = df->resolution[X];
		inner = df->resolution[Z];
		outerOffset = 1;
		innerOffset = df->resolution[X] * df->resolution[Y];
		clipOffset = df->resolution[X];
		clipSize = planeIdx * df->voxelSize[Y];
		outerSize = df->voxelSize[X];
		innerSize = df->voxelSize[Z];
		isInverse = (clipIdx == CPP_INV_Y) ? true : false;
		triClip = Y;
		triOuter = X;
		triInner = Z;
	}
	// z clip
	else if (clipIdx == CPP_Z || clipIdx == CPP_INV_Z) {
		if (planeIdx <= 0 || planeIdx >= df->resolution[Z] - 1) {
			vbo->numVertices = 0;
			return;
		}
		outer = df->resolution[X];
		inner = df->resolution[Y];
		outerOffset = 1;
		innerOffset = df->resolution[X];
		clipOffset = df->resolution[X] * df->resolution[Y];
		clipSize = planeIdx * df->voxelSize[Z];
		outerSize = df->voxelSize[X];
		innerSize = df->voxelSize[Y];
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
			if (df->data[index] >= df->innerThick && df->data[index] <= df->outerThick) {
				if (!precisionCompare(df->data[index], 0.0, PRECISION4)) {
					color[0] = 0.6;
					color[1] = 0.1;
					color[2] = 0.1;
					color[3] = 1.0;
				}
				else if (df->data[index] < 0) {
					color[0] = 0.2;
					color[1] = 0.2;
					color[2] = 0.6;
					color[3] = 1.0;
				}
				else if (df->data[index] > 0) {
					color[0] = 0.2;
					color[1] = 0.6;
					color[2] = 0.2;
					color[3] = 1.0;
				}
				else {
					color[0] = 0.0;
					color[1] = 0.0;
					color[2] = 0.0;
					color[3] = 1.0;
				}
				copy3fv(df->bbox->min, v0);
				copy3fv(df->bbox->min, v1);
				copy3fv(df->bbox->min, v2);
				copy3fv(df->bbox->min, v3);
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