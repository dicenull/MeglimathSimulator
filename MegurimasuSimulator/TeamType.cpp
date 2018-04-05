#include "TeamType.h"

Color Tile::ColorOf(TileType t_type)
{
	switch (t_type)
	{
	case TileType::A:
		return Palette::Red;
	case TileType::B:
		return Palette::Blue;
	case TileType::None:
	default:
		return Palette::White;
	}
}
