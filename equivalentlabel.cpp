#include "equivalentlabel.h"
#include <stdlib.h>

RootEquivalentLabel* newRootEquivalentLabelArray(int amount)
{
	RootEquivalentLabel* newRELArray = (RootEquivalentLabel*)malloc(sizeof(RootEquivalentLabel)*amount);

	for (int i = 0; i < amount; i++) {
		newRELArray[i] = newRootEquivalentLabel();
	}

	return newRELArray;
}

RootEquivalentLabel* resizeRootEquivalentLabelArray(RootEquivalentLabel* relArray, int oldAmount, int newAmount)
{
	relArray = (RootEquivalentLabel*)realloc(relArray, sizeof(RootEquivalentLabel)*newAmount);

	for (int i = oldAmount; i < newAmount; i++) {
		relArray[i] = newRootEquivalentLabel();
	}

	return relArray;
}

RootEquivalentLabel newRootEquivalentLabel()
{
	RootEquivalentLabel newREL;
	newREL.count = 0;
	newREL.headEL = newREL.tailEL = nullptr;

	return newREL;
}

EquivalentLabel* newEquivalentLabel(int id)
{
	EquivalentLabel* newEL = (EquivalentLabel*)calloc(sizeof(EquivalentLabel), 1);
	newEL->id = id;
	newEL->nextEL = nullptr;

	return newEL;
}

void addEquivalentLabel(RootEquivalentLabel* relArray, int ID)
{
	if (relArray->count == 0) {
		EquivalentLabel* newEL = newEquivalentLabel(ID);
		relArray->headEL = relArray->tailEL = newEL;
	}
	else {
		for (EquivalentLabel* oldEL = relArray->headEL; oldEL != nullptr; oldEL = oldEL->nextEL) {
			if (oldEL->id == ID) {
				return;
			}
		}
		EquivalentLabel* newEL = newEquivalentLabel(ID);
		relArray->tailEL->nextEL = newEL;
		relArray->tailEL = newEL;
	}
	relArray->count++;

	return;
}

void mergeRootEquivalentLabel(RootEquivalentLabel * mainREL, RootEquivalentLabel * moveREL)
{
	/*mainREL->count += moveREL->count;
	mainREL->tailEL->nextEL = moveREL->headEL;
	mainREL->tailEL = moveREL->tailEL;

	moveREL->count = 0;
	moveREL->headEL = moveREL->tailEL = nullptr;*/

	for (EquivalentLabel* moveCurrentEL = moveREL->headEL; moveCurrentEL != nullptr; moveCurrentEL = moveCurrentEL->nextEL) {
		bool repeat = false;
		for (EquivalentLabel* mainCurrentEL = mainREL->headEL; mainCurrentEL != nullptr; mainCurrentEL = mainCurrentEL->nextEL) {
			if (moveCurrentEL->id == mainCurrentEL->id) {
				repeat = true;
				break;
			}
		}
		if (repeat != true) {
			addEquivalentLabel(mainREL, moveCurrentEL->id);
		}
	}

	moveREL->count = 0;
	moveREL->headEL = moveREL->tailEL = nullptr; 

	return;
}

bool checkEquivalent(RootEquivalentLabel* rel, int ID)
{
	EquivalentLabel* currentEL = rel->headEL;
	for (EquivalentLabel* currentEL = rel->headEL; currentEL != nullptr; currentEL = currentEL->nextEL) {
		if (currentEL->id == ID) {
			return true;
		}
	}

	return false;
}
