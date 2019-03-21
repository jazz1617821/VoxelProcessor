#include "editvoxel.h"

EditVoxelData * newEditVoxelData(void)
{
	EditVoxelData* editVoxelData = (EditVoxelData*)calloc(sizeof(EditVoxelData), 1);
	editVoxelData->bbox = newAABB();

	return editVoxelData;
}

void deleteEditVoxelData(EditVoxelData** editdata)
{
	EditVoxelData* del;

	//if (del == NULL) return;

	del = (*editdata);

	// free raw data
	if (del->rawData != NULL) {
		free(del->rawData);
	}
	if (del->vbo != NULL) {
		deleteVBO(&del->vbo);
	}

	deleteAABB(&del->bbox);

	free(del);
	*editdata = NULL;
}
