#pragma once
#include <Siv3D.hpp>
#include "Types.h"
#include "Action.h"
#include "../TeamColor.h"

namespace Transform
{
	/// <summary>
	/// 方向から移動方向に変換する
	/// </summary>
	/// <param name="dir">変換元の方向</param>
	/// <returns>方向を変換した移動方向</returns>
	constexpr Point DirToDelta(Direction dir)
	{
		switch (dir)
		{
		case Direction::Right:
			return Point(1, 0);
			
		case Direction::RightUp:
			return Point(1, -1);
			
		case Direction::Up:
			return Point(0, -1);
			
		case Direction::LeftUp:
			return Point(-1, -1);
			
		case Direction::Left:
			return Point(-1, 0);

		case Direction::LeftDown:
			return Point(-1, 1);
			
		case Direction::Down:
			return Point(0, 1);
			
		case Direction::RightDown:
			return Point(1, 1);
			
		}
		return Point(0, 0);
	}

	/// <summary>
	/// タイルの種類から色を取得する
	/// </summary>
	/// <param name="t_type">タイルの種類</param>
	/// <returns>タイルの色</returns>
	constexpr Color ColorOf(TileType t_type)
	{
		switch (t_type)
		{
		case TileType::A:
			return TeamColor::A;
		case TileType::B:
			return TeamColor::B;
		case TileType::None:
		default:
			return Palette::White;
		}
	}

	constexpr Color ColorOf(TeamType t_type)
	{
		switch (t_type)
		{
		case TeamType::A:
			return TeamColor::A;
		case TeamType::B:
			return TeamColor::B;
		default:
			return Palette::Darkgray;
		}
	}

	const String ToString(Action action);

	const String ToString(Direction dir);

	const String ToString(Step step);

	const String ToString(TeamType team);

	const char ToString(TileType tile);
}