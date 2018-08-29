#pragma once
#include "Agent.h"
#include "Types.h"

struct Cell
{

	/// <summary>
	/// マスの点数
	/// </summary>
	int point = 0;

	// フィールド //
	/// <summary>
	/// マスにはめられたタイル
	/// </summary>
	TileType tile = TileType::None;

	// メソッド //

	/// <summary>
	/// 指定のチームによってセルにタイルが置かれる
	/// </summary>
	/// <param name="team">セルにタイルを置くチーム</param>
	void PaintedBy(TeamType team);

	/// <summary>
	/// 置かれたタイルを取る
	/// </summary>
	void RemoveTile();
};
