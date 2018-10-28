#pragma once
#include "Client.h"

/// <summary>
/// 指定した方向に進んだときの座標の変化分を返す
/// </summary>
inline _Point<int> DirectionToDeltaPos(Direction dir)
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
	default:
		return _Point<int>(0, 0);
	}
}

/// <summary>
/// 指定したフィールド内の指定したマスからみて指定した方向にあるタイルの色
/// </summary>
inline TileType GetTeamFromNext(Field field, _Point<int> pos, Step step)
{
	Direction dir = step.direction;
	_Point<int> pos_next = pos + DirectionToDeltaPos(dir);
	if (field.IsInField(pos_next))
		return field.cells[pos_next].tile;
	else
		return TileType::None;		//ダミー
}

/// <summary>
/// 指定したフィールド内の指定したマスからみて指定した方向にあるタイルの得点が負数かどうか
/// </summary>
inline bool GetIsNegativeFromNext(Field field, _Point<int> pos, Step step)
{
	Direction dir = step.direction;
	_Point<int> pos_next = pos + DirectionToDeltaPos(dir);
	if (field.IsInField(pos_next))
		return field.cells[pos_next].point < 0;
	else
		return true;	//ダミー
}

/// <summary>
/// フィールドの状態とエージェントの位置を見て、探索に有効と思われる Step を選択する。
/// </summary>
inline Array<Step> GetEssentialStep(Field field, TeamType team, _Point<int> pos, const int depth)
{
	Array<Step> essential_step = {};
	TileType thiss_tile = static_cast<TileType>(team);
	TileType others_tile = Transform::GetInverseTile(thiss_tile);

	for (auto act : { Action::Move, Action::RemoveTile })
	{
		for (auto i = 0; i < 8; i++)
		{
			auto dir = static_cast<Direction>(i);
			Step step = { act, dir };		// stepの生成を省く実装をすることでより高速化できるかもしれない
			TileType tile = GetTeamFromNext(field, pos, step);
			bool is_negative = GetIsNegativeFromNext(field, pos, step);

			if (tile == TileType::None && act == Action::RemoveTile)
				continue;
			if (depth <= 2)
			{
				if (tile == TileType::None && is_negative)
					continue;
			}
			if (tile == thiss_tile && !is_negative && act == Action::RemoveTile)
				continue;
			if (tile == others_tile && act == Action::Move)
				continue;
			if (act == Action::RemoveTile && dir == Direction::Stop)
				continue;

			essential_step.push_back({ act, dir });
		}
	}

	return essential_step;
}
