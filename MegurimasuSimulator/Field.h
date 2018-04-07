#pragma once
#include "Cell.h"
#include "Transform.h"

class Field
{
private:
	Grid<Cell> _cells;
	Point _origin;

	/// <summary>
	/// セル一つ当たりの大きさ
	/// </summary>
	const Size _c_size = Size(32, 32);

public:
	void Draw() const;

	Grid<Cell> GetCells();

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

public:
	Field();

	void operator=(const Field& other);

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="origin">原点</param>
	/// <param name="size">フィールドの大きさ</param>
	Field(Point origin, Size size);

	Field(Size size);
	virtual ~Field();
};
