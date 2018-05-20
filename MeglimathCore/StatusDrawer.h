#pragma once
#include <Siv3D.hpp>

class StatusDrawer
{
private:
	Point _origin;
	int _index = 0;
	const int _space_width = 3;

public:
	void DrawStatus(DrawableText text, Color text_color = Palette::Black, Color bg_color = Palette::White);

public:
	StatusDrawer(Point origin);
	~StatusDrawer();
};

