#pragma once
#include "TeamLogic.h"
#include "Transform.h"
#include <rapidjson\document.h>
#include <array>

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
	void PaintedBy(TeamType team)
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

	/// <summary>
	/// 置かれたタイルを取る
	/// </summary>
	void RemoveTile()
	{
		tile = TileType::None;
	}
};


struct GamePoints {
	int area, tile;
	int getTotal() const {
		return area + tile;
	}
};


class Field
{
public:
	/// <summary>
	/// フィールド情報
	/// </summary>
	_Grid<Cell> cells = { {6,6} };

private:
	/// <summary>
	/// 囲まれている領域を探索する
	/// </summary>
	/// <param name="pos">探索を開始する座標</param>
	/// <param name="tile">どのタイルで囲まれているか</param>
	void dfsAreaPoint(_Point<> pos, TileType tile, _Grid<bool>& _status)const;

	/// <summary>
	/// 指定のタイルで囲まれた領域の得点を集計します
	/// </summary>
	/// <param name="tile">どのタイルで囲まれているか</param>
	/// <returns>領域ポイント</returns>
	int aggregateAreaPoint(TileType tile)const;

	/// <summary>
	/// 指定のタイルのタイルポイントを集計します
	/// </summary>
	/// <param name="tile">得点を集計するタイル</param>
	/// <returns>タイルポイント</returns>
	int aggregateTilePoint(TileType tile)const;
	int aggregateTotalPoint(TileType tile)const;

public:

	std::array<int, 2> GetAreaPoints() const;
	std::array<int, 2> GetTilePoints() const;
	std::array<int, 2> GetTotalPoints() const;

	/// <summary>
	/// セルを塗る
	/// </summary>
	/// <param name="pos">塗るセルの座標</param>
	/// <param name="team">セルを塗るチーム</param>
	void PaintCell(_Point<> pos, TeamType team);

	/// <summary>
	/// タイルを取る
	/// </summary>
	/// <param name="pos">タイルの座標</param>
	void RemoveTile(_Point<> pos);

	/// <summary>
	/// 指定座標がフィールドの中であるか判定する
	/// </summary>
	/// <param name="pos">フィールド内か判定する座標</param>
	/// <returns>座標がフィールド内であるか</returns>
	bool IsInField(_Point<> pos) const;

	/// <summary>
	/// 座標と方向、フィールド状態から必要な行動を決める
	/// </summary>
	/// <param name="pos">指定方向に動く前の座標</param>
	/// <param name="dir">行動する方向</param>
	/// <returns>必要な行動</returns>
	Step DecideStepByDirection(_Point<> pos, Direction dir) const;

	Field MakeFieldFromStep(TeamType team, Agent agent, Step step);

	bool CanMove(_Point<> pos, TeamType team);
	bool CanRemoveTile(_Point<> pos, TeamType team);

public:
	static Field makeFieldFromJson(std::string json);
	static Field makeFieldRandom(_Size size = { 6,6 });

	bool IsSameStateField(const Field& other) const;
};