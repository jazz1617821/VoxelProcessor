#include "clipplane.h"
#include <stdlib.h>

ClipPlane *newClipPlane(float clip[4])
{
	ClipPlane *cpp;

	cpp = (ClipPlane*)calloc(1, sizeof(ClipPlane));

	cpp->plane[0] = clip[0];
	cpp->plane[1] = clip[1];
	cpp->plane[2] = clip[2];
	cpp->plane[3] = clip[3];

	return cpp;
}

void deleteClipPlane(ClipPlane **cpp)
{
	ClipPlane *del;

	del = *cpp;
	if (del == NULL)
		return;

	if (del->vbo != NULL) {
		deleteVBO(&del->vbo);
	}

	*cpp = NULL;
}