#pragma once
#include "Client.h"
#include <Siv3D.hpp>

class UIClient :
	public Client
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
	bool is_left = false;
	int idx = 0;

public:
	String Name() override { return U"UI"; }

	void Update(const GameInfo& info) override
	{
		if (IsReady())
		{
			return;
		}

		// 司令入力
		for (auto i : step(2))
		{
			if (coms[i].leftClicked())
			{
				is_left = (i == 0);
			}
		}

		auto w = field_ui.width();
		auto h = field_ui.height();
		for (int x = -1; x + 1 < 3;x++)
		{
			for (int y = -1;y + 1 < 3;y++)
			{
				if (x <= 0 || y <= 0 || x >= w || y >= h) continue;

				Point dp(x, y);
				for (auto i : step(2))
				{
					Point p = agent_points[i] + dp;
					if (field_ui[p.y][p.x].leftPressed())
					{
						_think.steps[idx].action =
							KeyControl.down() ? Action::RemoveTile : Action::Move;

						_think.steps[idx].direction =
							Transform::DeltaToDir(_Point(x, y));

						if (idx == 1) _is_ready = true;
						else idx++;

						return;
					}
				}
			}
		}
	}

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
					if (KeyControl.pressed()) r.draw(Palette::Red);
					else r.draw(Palette::Blue);
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

		for (auto i : step(2)) coms[i](com_imgs[i]).draw();
	}
public:
	UIClient(TeamType type);
};