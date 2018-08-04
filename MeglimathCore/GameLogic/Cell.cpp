#include "Cell.h"



void Cell::PaintedBy(TeamType team)
{
	switch (team)
	{
	case TeamType::A:
		_tile = TileType::A;
		break;

	case TeamType::B:
		_tile = TileType::B;
		break;

	}
}

void Cell::RemoveTile()
{
	_tile = TileType::None;
}

TileType Cell::GetTile() const
{
	return _tile;
}

int Cell::GetPoint() const
{
	return _point;
}

Cell::Cell()
	:Cell(0)
{}

Cell::Cell(int point)
{
	_tile = TileType::None;
	_point = point;
}


Cell::~Cell()
{
}
