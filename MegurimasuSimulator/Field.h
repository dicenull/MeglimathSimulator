#pragma once
#include "Cell.h"
#include "Transform.h"

// TODO: FieldGeneratorを作成

class Field
{
private:
	Grid<Cell> _cells;

public:
	/// <summary>
	/// フィールドをします
	/// </summary>
	/// <param name="file">フィールド情報のあるjsonファイル</param>
	/// <returns>生成したフィールド</returns>
	static Field Create(FilePath file);

public:
	Grid<Cell> GetCells() const;

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

	virtual ~Field();
};
