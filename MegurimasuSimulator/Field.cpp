#include "Field.h"

void Field::Draw() const
{
	Rect r;
	for (int i = 0; i < _cells.size().x; i++)
	{
		for (int k = 0; k < _cells.size().y; k++)
		{
			r = Rect(_origin + Point(i, k) * _c_size, _c_size);

			r.draw(Tile::ColorOf(_cells[k][i].GetTile()));
			r.drawFrame(1.0, Palette::Gray);

			_font(_cells[k][i].GetPoint()).drawAt(r.center(), Palette::Black);
		}
	}
}

Field::Field()
{
}

void Field::operator=(const Field & other)
{
	_cells = other._cells;
	_origin = other._origin;
}


Field::Field(Point origin, Size size)
{
	_origin = origin;
	_cells = Grid<Cell>(size);
}

Field::Field(Size size)
	:Field(Point(0, 0), size){}

Field::~Field()
{
}
