#pragma once
#include "Client.h"
#include <Siv3D.hpp>

class UIClient :
	public Client
{
private:
	Grid<Rect> field_ui;
	Grid<Color> field_color;
	Point agent_points[2];
	int idx = 0;
	Font font{ 20 };

private:
	void turn_init(const GameInfo& info);
	
public:
	String Name() override { return U"UI"; }

	void Update(const GameInfo& info) override
	{
		if (IsReady())
		{
			return;
		}
		turn_init(info);

		auto w = field_ui.width();
		auto h = field_ui.height();
		for (int x = -1; x + 1 < 3;x++)
		{
			for (int y = -1; y + 1 < 3; y++)
			{
				Point dp(x, y);
				Point p = agent_points[idx] + dp;

				if (p.x < 0 || p.y < 0 || p.x >= w || p.y >= h) continue;

				auto & f = field_ui[p.y][p.x];
				auto l = f.leftReleased();
				auto r = f.rightReleased();
				if(l || r)
				{
					if (r) _think.steps[idx].action = Action::RemoveTile;

					if (l) _think.steps[idx].action = Action::Move;

					_think.steps[idx].direction =
						Transform::DeltaToDir(_Point(x, y));

					if (idx == 1)
					{
						_is_ready = true;
						idx = 0;
					}
					else idx++;

					return;
				}
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
				r.draw(field_color[y][x]);

				// 移動、削除入力
				if (r.leftPressed())
				{
					r.draw(Palette::Blue);
				}
				
				if (r.rightPressed())
				{
					r.draw(Palette::Red);
				}

				for (auto i : step(2))
				{
					if (agent_points[i] == Point(x, y))
					{
						Circle(r.center(), 15).draw();
						font(i + 1).drawAt(r.center(), Palette::Black);
					}
				}

			}
		}
	}
public:
	UIClient(TeamType type);
};