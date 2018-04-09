#include "FieldDrawer.h"


void FieldDrawer::SetOrigin(Point origin) 
{
	_origin = origin;
}

Point FieldDrawer::GetOrigin() const
{
	return _origin;
}

void FieldDrawer::Draw(const Field & field) const
{
	Rect r;
	auto & cells = field.GetCells();
	for (int i = 0; i < cells.size().x; i++)
	{
		for (int k = 0; k < cells.size().y; k++)
		{
			r = Rect(_origin + Point(i, k) * _c_size, _c_size);

			r.draw(Transform::ColorOf(cells[k][i].GetTile()));
			r.drawFrame(1.0, Palette::Gray);

			FontAsset(U"Cell")(cells[k][i].GetPoint()).drawAt(r.center(), Palette::Black);
		}
	}
}

FieldDrawer::FieldDrawer()
{
}


FieldDrawer::~FieldDrawer()
{
}

void FieldDrawer::operator=(const FieldDrawer & other)
{
	_origin = other._origin;
}
