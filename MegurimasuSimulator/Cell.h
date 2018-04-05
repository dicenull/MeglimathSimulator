#pragma once
#include "Agent.h"
#include "TeamType.h"

class Cell
{
private:
	/// <summary>
	/// マスにはめられたタイル
	/// </summary>
	TileType _tile;

	/// <summary>
	/// マスの点数
	/// </summary>
	int _point;

public:
	/// <summary>
	/// 指定のチームによってセルにタイルが置かれる
	/// </summary>
	/// <param name="team">セルにタイルを置くチーム</param>
	void PaintedBy(TeamType team);

	/// <summary>
	/// セルにはめられたタイルを取得する
	/// </summary>
	/// <returns>セルにあるタイル</returns>
	TileType GetTile() const;

	/// <summary>
	/// セルの得点を取得する
	/// </summary>
	/// <returns>セルの得点</returns>
	int GetPoint() const;

public:
	Cell();
	Cell(int point);
	virtual ~Cell();

};
