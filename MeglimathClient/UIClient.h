#pragma once
#include "ManualClient.h"
#include <Siv3D.hpp>

class UIClient :
	public ManualClient
{
private:
	const Texture const com_imgs[2] =
	{
		Texture(U"image/com_right.PNG"),
		Texture(U"image/com_left.PNG")
	};
	Rect coms[2] = { Rect(com_imgs[0].size()), Rect(com_imgs[1].size()) };
	Grid<Rect> field_ui;
	Point agent_points[2];

public:
	String Name() override { return U"UI"; }

	void Initialize(const GameInfo& info) override
	{
		auto agents = info.GetAgents(Transform::GetInverseTeam(_type));
		for (auto i : step(2))
		{
			agent_points[i] = { agents[i].position.x, agents[i].position.y };
		}

		auto field = info.GetField();
		Size size = { field.cells.width(), field.cells.height() };

		if (field_ui.size() == size)
		{
			return;
		}

		// UI初期化
		field_ui = Grid<Rect>(size);
		Size draw_size(30, 30);
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

	bool IsDraw() override { return true; }

	void Draw() override
	{
		for (int y = 0; y < field_ui.height(); y++)
		{
			for (int x = 0; x < field_ui.width(); x++)
			{
				auto &r = field_ui[y][x];

				r.drawFrame();

				// 移動、削除入力
				if (r.leftPressed())
				{
					if (KeyControl.pressed())
					{
						r.draw(Palette::Red);
					}
					if (KeyLShift.pressed())
					{
						r.draw(Palette::Blue);
					}
				}

				for (auto i : step(2))
				{
					if (agent_points[0] == Point(x, y))
					{
						Circle(r.center(), 10).draw();
					}
				}

			}
		}
	}
public:
	UIClient(TeamType type);
};