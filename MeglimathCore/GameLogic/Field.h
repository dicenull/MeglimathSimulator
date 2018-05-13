#pragma once
#include "Cell.h"
#include "Transform.h"
#include "GamePoints.h"

class Field
{
private:
	/// <summary>
	/// フィールド情報
	/// </summary>
	_Grid<Cell> _cells;

	/// <summary>
	/// 領域ポイントを計算するとき探索したかを格納する
	/// </summary>
	_Grid<bool> _status;

	GamePoints _points[2];

private:
	/// <summary>
	/// 囲まれている領域を探索する
	/// </summary>
	/// <param name="pos">探索を開始する座標</param>
	/// <param name="tile">どのタイルで囲まれているか</param>
	void dfsAreaPoint(_Point<> pos, TileType tile);

	/// <summary>
	/// 指定のタイルで囲まれた領域の得点を集計します
	/// </summary>
	/// <param name="tile">どのタイルで囲まれているか</param>
	/// <returns>領域ポイント</returns>
	int aggregateAreaPoint(TileType tile);

	/// <summary>
	/// 指定のタイルのタイルポイントを集計します
	/// </summary>
	/// <param name="tile">得点を集計するタイル</param>
	/// <returns>タイルポイント</returns>
	int aggregateTilePoint(TileType tile);

	int aggregateTotalPoint(TileType tile);

public:

	/// <summary>
	/// タイルと領域のポイントを集計し、データを更新する
	/// </summary>
	void UpdatePoint();

	/// <summary>
	/// セル情報を取得する
	/// </summary>
	/// <returns>フィールドのセル情報</returns>
	_Grid<Cell> GetCells() const;

	std::vector<int> GetAreaPoints() const;
	std::vector<int> GetTilePoints() const;
	std::vector<int> GetTotalPoints() const;

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

public:
	Field();

	void operator=(const Field& other);

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="size">フィールドの大きさ</param>
	Field(_Size size);

	/// <summary>
	/// セルを元にフィールドを生成するコンストラクタ
	/// </summary>
	/// <param name="cells">元となるセル</param>
	Field(_Grid<Cell> cells);

	/// <summary>
	/// フィールドを生成します
	/// </summary>
	/// <param name="file">フィールド情報のあるjsonファイル</param>
	Field(std::string json);

	virtual ~Field();
};
