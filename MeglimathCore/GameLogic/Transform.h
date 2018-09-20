#pragma once
#include "Types.h"
#include "Action.h"


namespace Transform
{
	
	/// <summary>
	/// 方向から移動方向に変換する
	/// </summary>
	/// <param name="dir">変換元の方向</param>
	/// <returns>方向を変換した移動方向</returns>
	constexpr _Point<int> DirToDelta(Direction dir)
	{
		switch (dir)
		{
		case Direction::Right:
			return _Point<int>(1, 0);
			
		case Direction::RightUp:
			return _Point<int>(1, -1);
			
		case Direction::Up:
			return _Point<int>(0, -1);
			
		case Direction::LeftUp:
			return _Point<int>(-1, -1);
			
		case Direction::Left:
			return _Point<int>(-1, 0);

		case Direction::LeftDown:
			return _Point<int>(-1, 1);
			
		case Direction::Down:
			return _Point<int>(0, 1);
			
		case Direction::RightDown:
			return _Point<int>(1, 1);
			
		}
		return _Point<int>(0, 0);
	}

	constexpr TileType ToTile(TeamType team)
	{
		switch (team)
		{
		case TeamType::A:
			return TileType::A;

		case TeamType::B:
			return TileType::B;
		}

		return TileType::None;
	}

	constexpr TileType GetInverseTile(TileType tile)
	{
		switch (tile)
		{
		case TileType::A:
			return TileType::B;
		case TileType::B:
			return TileType::A;
		}

		return TileType::None;
	}

	constexpr TeamType GetInverseTeam(TeamType team)
	{
		switch (team)
		{
		case TeamType::A:
			return TeamType::B;
		case TeamType::B:
			return TeamType::A;
		}
	}

	const std::u32string ToString(Action action);

	const std::u32string ToString(Direction dir);

	const std::u32string ToString(Step step);

	const std::u32string ToString(TeamType team);

	const char ToChar(TileType tile);
}