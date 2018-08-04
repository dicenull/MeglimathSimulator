#include "StatusDrawer.h"



void StatusDrawer::DrawStatus(DrawableText text, Color text_color, Color bg_color)
{
	auto region = text.region();
	Point draw_pos = _origin + Point(0, _index * (region.size.y + _space_width));

	text.region(draw_pos).draw(bg_color);
	text.draw(draw_pos, text_color);

	_index++;
}

StatusDrawer::StatusDrawer(Point origin)
{
	_origin = origin;
}


StatusDrawer::~StatusDrawer()
{
}
