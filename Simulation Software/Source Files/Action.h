#pragma once

#include "GraphicalElement.h"

class Action {
private:
	ElementKey m_elementID;

public:
	virtual void redo() = 0;
	virtual void undo() = 0;
};

class MoveAction {
private:
	
public:

};