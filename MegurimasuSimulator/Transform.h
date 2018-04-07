#pragma once
#include <Siv3D.hpp>
#include "TeamType.h"
#include "Action.h"

namespace Transform
{
	constexpr Point DirToDelta(Direction dir)
	{
		Point movement = Point(0, 0);
		switch (dir)
		{
		case Direction::Right:
			movement = Point(1, 0);
			break;
		case Direction::RightUp:
			movement = Point(1, -1);
			break;
		case Direction::Up:
			movement = Point(0, -1);
			break;
		case Direction::LeftUp:
			movement = Point(-1, -1);
			break;
		case Direction::Left:
			movement = Point(-1, 0);
			break;
		case Direction::Down:
			movement = Point(0, 1);
			break;
		case Direction::RightDown:
			movement = Point(1, 1);
			break;
		}

		return movement;
	}

	/// <summary>
	/// É^ÉCÉãÇÃêFÇéÊìæÇ∑ÇÈ
	/// </summary>
	constexpr Color ColorOf(TileType t_type)
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
}