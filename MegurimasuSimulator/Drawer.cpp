#include "Drawer.h"

void Drawer::DrawField(const Field & field) const
{
	// セルとタイルポイントの描画
	Rect r;
	
	auto cells = field.GetCells();
	
	for (int i = 0; i < cells.size().x; i++)
	{
		for (int k = 0; k < cells.size().y; k++)
		{
			Point pos(i, k);
			Point top_left_pos = fieldOrigin + pos * cellSize;
			
			r = Rect(top_left_pos, cellSize);

			r.draw(Palette::White);

			if (cells[k][i].GetTile() != TileType::None)
			{
				r.draw(Color(Transform::ColorOf(cells[k][i].GetTile()), 50U));
			}
			
			r.drawFrame(1.0, Palette::Gray);
			
			FontAsset(U"Cell")(cells[k][i].GetPoint())
				.drawAt(r.center(), Palette::Black);
		}
	}
}

void Drawer::DrawAgents(std::map<TeamType, Array<Agent>> agents) const
{
	int32 edge_width = Sqrt(2) * cellSize.x / 2.0;

	auto center = [=](Point pos) {return fieldOrigin + pos * cellSize + cellSize / 2; };
	for(TeamType team : {TeamType::A, TeamType::B})
	{
		// 一人目のエージェントを描画
		Circle(center(agents[team][0].GetPosition()), cellSize.x / 2)
			.drawFrame(2.0, Transform::ColorOf(team));

		// 二人目のエージェントを描画
		Rect(Arg::center = center(agents[team][1].GetPosition()), edge_width).rotated(45_deg)
			.drawFrame(2.0, Transform::ColorOf(team));
	}
}

void Drawer::DrawStatus(const std::map<TeamType, Think> & thinks, const Field & field, int turn) const
{
	if (thinks.size() == 0)
	{
		return;
	}

	Array<Array<String>> messages{ 3 };

	// 2チームの情報
	for (int i : step(2))
	{
		messages[i].append
		({
			String(U"Agent 1 : ") + Transform::ToString(thinks.at((TeamType)i).steps[0]),
			String(U"Agent 2 : ") + Transform::ToString(thinks.at((TeamType)i).steps[1]),
			String(U"Area Point : ") + ToString(field.GetAreaPoints()[i]),
			String(U"Tile Point : ") + ToString(field.GetTilePoints()[i]),
			String(U"Total Point : ") + ToString(field.GetTotalPoints()[i])
		});
	}

	// その他のゲーム情報
	messages[2].push_back(String(U"Turn : ") + ToString(turn));

	// ステータスを描画
	int index = 0;
	Color team_colors[] = { Transform::ColorOf(TeamType::A), Transform::ColorOf(TeamType::B) };

	auto draw_pos = [&](Point origin) {return origin + Point(0, index * 24); };
	for (int i : step(2))
	{
		auto text = FontAsset(U"Stat")(Transform::ToString((TeamType)i));
		text.region(draw_pos(statOrigin)).draw(Palette::Gray);
		text.draw(draw_pos(statOrigin), team_colors[i]);

		index++;

		for (size_t k : step(messages[i].count()))
		{
			text = FontAsset(U"Stat")(messages[i][k]);
			text.region(draw_pos(statOrigin)).draw(Palette::Gray);
			text.draw(draw_pos(statOrigin), Transform::ColorOf((TeamType)i));
			index++;
		}
	}

	for (size_t i : step(messages[2].count()))
	{
		FontAsset(U"Stat")(messages[2][i])
			.draw(draw_pos(statOrigin));
		index++;
	}
}

Drawer::Drawer()
{
}


Drawer::~Drawer()
{
}
