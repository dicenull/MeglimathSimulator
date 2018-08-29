#include "Cell.h"



void Cell::PaintedBy(TeamType team)
{
	switch (team)
	{
	case TeamType::A:
		tile = TileType::A;
		break;

	case TeamType::B:
		tile = TileType::B;
		break;
	}
}

void Cell::RemoveTile()
{
	tile = TileType::None;
}