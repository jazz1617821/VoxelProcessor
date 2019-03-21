#pragma once
#ifndef _EQUIVALENTLABEL_H
#define _EQUIVALENTLABEL_H

struct EquivalentLabel {
	int id;
	EquivalentLabel* nextEL;
};

struct RootEquivalentLabel {
	int count;
	EquivalentLabel* headEL;
	EquivalentLabel* tailEL;
};

RootEquivalentLabel* newRootEquivalentLabelArray(int amount);
RootEquivalentLabel* resizeRootEquivalentLabelArray(RootEquivalentLabel* REL,int oldAmount,int newAmount);
//RootEquivalentLabel* deleteEquivalentLabelArray(RootEquivalentLabel* REL);
RootEquivalentLabel newRootEquivalentLabel();
EquivalentLabel* newEquivalentLabel(int id);
//EquivalentLabel* deleteEquivalentLabel();
void addEquivalentLabel(RootEquivalentLabel* REL,int ID);
void mergeRootEquivalentLabel(RootEquivalentLabel* mainREL, RootEquivalentLabel* moveREL);
bool checkEquivalent(RootEquivalentLabel* rel,int ID);


#endif // !_EQUIVALENTLABEL_H



