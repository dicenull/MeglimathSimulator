#include "UIClient.h"



void UIClient::turn_init(const GameInfo & info)
{
	// my
	auto agents = info.GetAgents(type);
	for (auto i : step(2))
	{
		agent_points[i] = { agents[i].position.x, agents[i].position.y };
	}

	// other
	agents = info.GetAgents(TeamType(1 - type));
	for (auto i : step(2))
	{
		other[i] = { agents[i].position.x, agents[i].position.y };
	}

	auto& field = info.GetField();
	Size size = { field.cells.width(), field.cells.height() };

	// 色更新
	for (auto y = 0; y < field_color.height(); y++)
	{
		for (auto x = 0; x < field_color.width(); x++)
		{
			switch (field.cells[_Point<>(x, y)].tile)
			{
			case TileType::Red:
				field_color[y][x] = Color(Palette::Blue, 50U);
				break;
			case TileType::Blue:
				field_color[y][x] = Color(Palette::Red, 50U);
				break;
			case TileType::None:
				field_color[y][x] = Color(Palette::White, 50U);
				break;
			}
		}
	}

	if (field_ui.size() == size)
	{
		return;
	}

	// UI初期化
	field_ui = Grid<Rect>(size);
	field_color.resize(size);

	// 四角形更新
	Point draw_size = Window::Size() / size;
	for (auto y = 0; y < size.y; y++)
	{
		for (auto x = 0; x < size.x; x++)
		{
			Point pos(x, y);
			Point draw_pos = Point(pos * draw_size);

			field_ui[y][x] = Rect(draw_pos, draw_size);
		}
	}
}

UIClient::UIClient(TeamType type)
{
	type = type;
}
