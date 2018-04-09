#include "Field.h"

void Field::Draw()const
{
	_drawer.Draw(this);
}

Grid<Cell> Field::GetCells() const
{
	return _cells;
}

void Field::PaintCell(Point pos, TeamType team)
{
	_cells[pos.y][pos.x].PaintedBy(team);
}

void Field::RemoveTile(Point pos)
{
	_cells[pos.y][pos.x].RemoveTile();
}

Field::Field()
{
}

void Field::operator=(const Field & other)
{
	_cells = other._cells;
	_drawer = other._drawer;
}


Field::Field(Point origin, Size size)
{
	_drawer.SetOrigin(origin);
	_cells = Grid<Cell>(size);
}

Field::Field(Size size)
	:Field(Point(0, 0), size){}

Field::~Field()
{
}
