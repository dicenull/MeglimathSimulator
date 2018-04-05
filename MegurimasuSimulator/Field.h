#pragma once
#include "Cell.h"

class Field
{
private:
	Grid<Cell> _cells;
	Point _origin;

	/// <summary>
	/// セル一つ当たりの大きさ
	/// </summary>
	const Size _c_size = Size(32, 32);

	const Font _font{ _c_size.x / 2 };

public:
	void Draw() const;


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
