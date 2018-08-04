#include "GamePoints.h"



int GamePoints::GetArea() const
{
	return _area;
}

int GamePoints::GetTile() const
{
	return _tile;
}

int GamePoints::GetTotal() const
{
	return _area + _tile;
}

GamePoints::GamePoints()
{
	_area = _tile = 0;
}

GamePoints::GamePoints(int area, int tile)
{
	_area = area;
	_tile = tile;
}


GamePoints::~GamePoints()
{
}
