#pragma once
#include "Cell.h"

class Field
{
private:
	Grid<Cell> _cells;

public:
	Field();
	virtual ~Field();
};
