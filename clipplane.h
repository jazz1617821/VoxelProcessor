#ifndef _CLIP_PLANE_H
#define _CLIP_PLANE_H

#include "vbo.h"
#include <stdbool.h>

typedef struct VBO VBO;

typedef struct ClipPlane {
	bool isEnable;
	float plane[4];
	VBO *vbo;
}ClipPlane;

ClipPlane *newClipPlane(float clip[4]);
void deleteClipPlane(ClipPlane **cpp);

#endif // _CLIP_PLANE_H
