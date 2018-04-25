#pragma once
#include "Cell.h"
#include "Transform.h"

class Field
{
private:
	/// <summary>
	/// フィールド情報
	/// </summary>
	Grid<Cell> _cells;

	/// <summary>
	/// 領域ポイントを計算するとき探索したかを格納する
	/// </summary>
	Grid<bool> _status;

	int _total_point[2];

private:
	/// <summary>
	/// 囲まれている領域を探索する
	/// </summary>
	/// <param name="pos">探索を開始する座標</param>
	/// <param name="tile">どのタイルで囲まれているか</param>
	void dfsAreaPoint(Point pos, TileType tile);

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
	Grid<Cell> GetCells() const;

	Array<int> GetTotalPoint() const;

	/// <summary>
	/// セルを塗る
	/// </summary>
	/// <param name="pos">塗るセルの座標</param>
	/// <param name="team">セルを塗るチーム</param>
	void PaintCell(Point pos, TeamType team);

	/// <summary>
	/// タイルを取る
	/// </summary>
	/// <param name="pos">タイルの座標</param>
	void RemoveTile(Point pos);

	/// <summary>
	/// 指定座標がフィールドの中であるか判定する
	/// </summary>
	/// <param name="pos">フィールド内か判定する座標</param>
	/// <returns>座標がフィールド内であるか</returns>
	bool IsInField(Point pos);

	/// <summary>
	/// 座標と方向、フィールド状態から必要な行動を決める
	/// </summary>
	/// <param name="pos">指定方向に動く前の座標</param>
	/// <param name="dir">行動する方向</param>
	/// <returns>必要な行動</returns>
	Step DecideStepByDirection(Point pos, Direction dir) const;

public:
	Field();

	void operator=(const Field& other);

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="size">フィールドの大きさ</param>
	Field(Size size);

	/// <summary>
	/// セルを元にフィールドを生成するコンストラクタ
	/// </summary>
	/// <param name="cells">元となるセル</param>
	Field(Grid<Cell> cells);

	/// <summary>
	/// フィールドを生成します
	/// </summary>
	/// <param name="file">フィールド情報のあるjsonファイル</param>
	Field(FilePath file);

	virtual ~Field();
};
