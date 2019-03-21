#include "voxel.h"

#define X 0
#define Y 1
#define Z 2

// create voxel data
VoxelData *newVoxelData(void)
{
	int i;
	VoxelData *vdata;

	// allocate memory for vdata structure, and set the values to zero
	vdata = (VoxelData*)calloc(1, sizeof(VoxelData));
	vdata->bbox = newAABB();

	return vdata;
}

void deleteVoxelData(VoxelData **vdata)
{
	VoxelData *del;

	if (del == NULL) return;

	del = (*vdata);

	// free raw data
	if (del->rawData != NULL) {
		free(del->rawData);
	}
	if (del->df != NULL) {
		deleteDistanceField(&del->df);
	}
	if (del->vbo != NULL) {
		deleteVBO(&del->vbo);
	}
	/*
	// delete trivox
	if (del->lsArray != NULL) {

	}
	*/

	deleteAABB(&del->bbox);
	/*
	if (del->originalDataType == OD_TYPE_MESH_MODEL) {
	deleteModel(&del->model);
	}
	*/

	// set the pointer to null
	free(del);
	*vdata = NULL;
}

VoxelItem *newVoxelItemAnchor(void)
{
	VoxelItem *item;

	item = (VoxelItem*)calloc(1, sizeof(VoxelItem));
	initEnqDList(&item->link);
	item->parent = NULL;
	item->vdata = NULL;

	return item;
}

VoxelItem *newVoxelItem(VoxelData *vdata, VoxelObject *vobj)
{
	VoxelItem *item;

	item = (VoxelItem*)calloc(1, sizeof(VoxelItem));
	initEnqDList(&item->link);
	item->parent = vobj;
	item->vdata = vdata;

	return item;
}

void clearVoxelItems(VoxelItem *items)
{
	VoxelItem *delItem;

	if (items->vdata != NULL)
		return;

	// delete all items
	while (!isEmptyVoxelItem(items))
	{
		delItem = popBackVoxelItem(items);
		free(delItem);
	}

	// link to itself
	items->link.flink = &(items->link);
	items->link.blink = &(items->link);
}

void deleteVoxelItem(VoxelItem **item)
{
	if ((*item)->vdata == NULL) {
		if (isEmptyVoxelItem((*item))) {
			free(*item);
			*item = NULL;
		}
		return;
	}
	// link the left and right items
	(*item)->link.blink->flink = (*item)->link.flink;
	(*item)->link.flink->blink = (*item)->link.blink;
	free(*item);
	*item = NULL;
}

void deleteVoxelItems(VoxelItem **items)
{
	VoxelItem *delItem;

	if (items == NULL)
		return;

	clearVoxelItems(*items);
	free(*items);
	*items = NULL;
}

VoxelItem *firstVoxelItem(VoxelItem *items)
{
	if (items->vdata != NULL)
		return NULL;

	return (VoxelItem*)items->link.flink;
}

VoxelItem *lastVoxelItem(VoxelItem *items)
{
	if (items->vdata != NULL)
		return NULL;

	return (VoxelItem*)items->link.blink;
}

VoxelItem * nextVoxelItem(VoxelItem * item)
{
	return (VoxelItem*)item->link.flink;
}

VoxelItem * prevVoxelItem(VoxelItem * item)
{
	return (VoxelItem*)item->link.blink;
}

void pushFrontVoxelItem(VoxelItem *items, VoxelItem *item)
{
	if (items->vdata != NULL)
		return;

	addEnqHead(&item->link, &item->link);
}

void pushBackVoxelItem(VoxelItem *items, VoxelItem *item)
{
	if (items->vdata != NULL)
		return;

	addEnqTail(&items->link, &item->link);
}

VoxelItem *popFrontVoxelItem(VoxelItem *items)
{
	if (items->vdata != NULL)
		return NULL;

	return (VoxelItem*)removeEnqHead(&items->link);
}

VoxelItem *popBackVoxelItem(VoxelItem *items)
{
	if (items->vdata != NULL)
		return NULL;

	return (VoxelItem*)removeEnqTail(&items->link);
}

bool isAnchorVoxelItem(VoxelItem *items)
{
	return items->vdata == NULL ? true : false;
}

bool isEmptyVoxelItem(VoxelItem *items)
{
	return isEmptyEnq(&items->link);
}

// create voxel object
VoxelObject *newVoxelObject(void)
{
	VoxelObject *vobj;

	// allocate memory for vobj data structure, and set the values to zero
	vobj = (VoxelObject*)calloc(1, sizeof(VoxelObject));
	vobj->bbox = newAABB();

	return vobj;
}

// delete voxel object without recursion.
// record the firstChild in each level of the tree structure in an 1D array.
// delete all vobj from sibling list.
void deleteVoxelObject(VoxelObject **vobj)
{

}

void insertVoxelObject(VoxelObject *root, VoxelObject *obj)
{
	insertObjectTreeNode(&root->node, &obj->node);
}

void removeVoxelObject(VoxelObject *obj)
{
	removeObjectTreeNode(&obj->node);
}

VoxelObject *parentOfVoxelObject(VoxelObject *obj)
{
	return (VoxelObject*)obj->node.parent;
}

VoxelObject *childOfVoxelObject(VoxelObject *obj)
{
	return (VoxelObject*)obj->node.firstChild;
}

VoxelObject *nextSiblingOfVoxelObject(VoxelObject *obj)
{
	return (VoxelObject*)obj->node.fsibling;
}

VoxelObject *prevSiblingOfVoxelObject(VoxelObject *obj)
{
	return (VoxelObject*)obj->node.bsibling;
}

Dequeue *bfVoxelObject(VoxelObject *obj)
{
	return (VoxelObject*)bfObjectTreeNode(&obj->node);
}

Dequeue *dfVoxelObject(VoxelObject *obj)
{
	return (VoxelObject*)dfObjectTreeNode(&obj->node);
}

Dequeue *firstNodesOfVoxelObject(VoxelObject *obj)
{
	return firstNodesOfModelObject(&obj->node);
}

Dequeue *leafNodesOfVoxelObject(VoxelObject *obj)
{
	return leafNodesOfObjectTree(&obj->node);
}

int levelOfVoxelObject(VoxelObject *obj)
{
	return levelOfObjectTree(&obj->node);
}

bool isRootVoxelObject(VoxelObject *obj)
{
	return isRootObjectTreeNode(&obj->node);
}

bool isLeafVoxelObject(VoxelObject *obj)
{
	return isLeafObjectTreeNode(&obj->node);
}

void calVoxelObjectBBox(VoxelObject *root)
{
	VoxelObject *childObject, *parentObject, *shownParent;
	Dequeue *dequeue;

	// breadth-first inserted dequeue of object tree
	dequeue = bfObjectTreeNode(&root->node);

	// pop from back of deqeueu (leaf nodes first)
	shownParent = NULL;
	while (!isEmptyDequeue(dequeue))
	{
		childObject = (VoxelObject*)popBackDequeue(dequeue);

		parentObject = (VoxelObject*)childObject->node.parent;
		if (parentObject == NULL)
			break;

#ifdef _DEBUG
		printf("parentObject: %s\n", parentObject->name);
		printf("childObject: %s\n", childObject->name);
#endif
		// initialize parent node's bbox
		if (shownParent != parentObject) {
			shownParent = parentObject;
			initAABB(shownParent->bbox);
		}
		// re-calculate bounding box
		addAABB(shownParent->bbox, childObject->bbox);
#ifdef _DEBUG
		printf("update bbox\n");
		printf("min: %f %f %f\n", shownParent->bbox->min[X],
			shownParent->bbox->min[Y],
			shownParent->bbox->min[Z]);
		printf("max: %f %f %f\n", shownParent->bbox->max[X],
			shownParent->bbox->max[Y],
			shownParent->bbox->max[Z]);
#endif // _DEBUG

	}
	deleteDequeue(&dequeue);

#ifdef _DEBUG
	printf("calVoxelObjectBBox\n");
	printf("min: %f %f %f\n", root->bbox->min[X],
		root->bbox->min[Y],
		root->bbox->min[Z]);
	printf("max: %f %f %f\n", root->bbox->max[X],
		root->bbox->max[Y],
		root->bbox->max[Z]);
	printf("end\n");
#endif // !_DEBUG 

}

void updateVoxelObjectBBox(VoxelObject *obj)
{

}

// create voxel model
VoxelModel *newVoxelModel(void)
{
	VoxelModel *vmodel;

	// allocate memory for vmodel structure, and set the values to zero
	vmodel = (VoxelModel*)calloc(1, sizeof(VoxelModel));
	vmodel->items = newVoxelItemAnchor();
	vmodel->root = newVoxelObject();
	strcpy(vmodel->root->name, "root");

	return vmodel;
}

void deleteVoxelModel(VoxelModel **vmodel)
{
	if ((*vmodel) == NULL) return;

	if ((*vmodel)->root != NULL) {
		deleteVoxelObject(&(*vmodel)->root);
	}

	free(*vmodel);
	*vmodel = NULL;
}

void delVoxelObjectFromScene(VoxelModel *sc, VoxelObject *obj)
{
	Dequeue *leafNodes;
	VoxelObject *objNode;

	// remove model object to the scene graph
	removeVoxelObject(obj);
	// re-calculate the bounding box of all objects
	updateVoxelObjectBBox(parentOfVoxelObject(obj));

	// remove model to the item list
	leafNodes = leafNodesOfVoxelObject(obj);
	while (!isEmptyDequeue(leafNodes))
	{
		objNode = (VoxelObject*)popFrontDequeue(leafNodes);
		if (isLeafModelObject(objNode)) {
			delVoxelItemFromScene(sc, objNode->dataItem);
		}
	}
	deleteDequeue(&leafNodes);
	deleteVoxelObject(&obj);
}

void delVoxelItemFromScene(VoxelModel *sc, VoxelItem *item)
{
	VoxelItem *delItem;
	VoxelData *delData;

	if (item == NULL)
		return;

	delItem = firstVoxelItem(sc->items);
	while (delItem != item) {
		delItem = nextVoxelItem(delItem);
	}
	delData = delItem->vdata;

	// free model data
	// Must delete model item first
	// if delete the model, it will be regard as a anchor item
	// that will cause error link state
	deleteVoxelItem(&delItem);
	deleteVoxelData(&delData);

	sc->numberOfVoxelData--;
}

VoxelItem *searchVoxelData(int id, VoxelModel *vmodel)
{
	VoxelItem *item;

	item = firstVoxelItem(vmodel->items);
	while (item->vdata != NULL)
	{
		if (item->vdata->id == id)
			return item;
		item = nextVoxelItem(item);
	}
	return NULL;
}