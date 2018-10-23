#pragma once
#include <Siv3D.hpp>
#include"GameLogic/GameLogic.h"
namespace TeamColor
{
	constexpr Color A = Palette::Blue;
	constexpr Color B = Palette::Red;

	/// <summary>
	/// タイルの種類から色を取得する
	/// </summary>
	/// <param name="t_type">タイルの種類</param>
	/// <returns>タイルの色</returns>
	constexpr Color ColorOf(TileType t_type)
	{
		switch (t_type)
		{
		case TileType::Blue:
			return TeamColor::A;
		case TileType::Red:
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
		case TeamType::Blue:
			return TeamColor::A;
		case TeamType::Red:
			return TeamColor::B;
		default:
			return Palette::Darkgray;
		}
	}
}
