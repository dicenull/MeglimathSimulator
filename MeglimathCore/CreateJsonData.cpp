#include "CreateJsonData.h"
#include <Siv3D.hpp>

const String Transform::ToJSON(const GameInfo& info)
{
	String json = U"{\r\n";

	Grid<Cell> cells = info.GetField.GetCells();

	// データを書き込む
	// 大きさ
	json += String(U"\"Size\" : \"" + Format(cells.size()) + U"\",\r\n");
		
	// タイルポイント
	json += String(U"\"Cells\" : [\r\n");
	Size size = { (cells.size().x + 1) / 2, (cells.size().y + 1) / 2 };
	for (int y : step(size.y))
	{
		String cell_arr;
		for (int x : step(size.x))
		{
			if (x > 0)
			{
				cell_arr.push_back(U',');
			}

			cell_arr += Format(cells[y][x].GetPoint());
		}

		if(y < size.y - 1)
		{
			cell_arr.push_back(U',');
		}

		json += String(cell_arr + U"\r\n");
	}
	json += U"],\r\n";

	// タイル取得状況
	json += String(U"Tiles : [\r\n");
	for (int y : step(size.y))
	{
		for (int x : step(size.x))
		{
			switch (cells[y][x].GetTile())
			{
			case TileType::A:
				json.push_back(U'a');
				break;
			case TileType::B:
				json.push_back(U'b');
				break;
			default:
				json.push_back(U' ');
			}
		}

		if (y < size.y - 1)
		{
			json.push_back(U',');
		}
		else
		{
			json += U"],";
		}
	}

	auto pos_to_json = [](Point p1, Point p2) {return U"[\"" + Format(p1) + U"\", \"" + Format(p2) + U"\"],\r\n"; };
	Array<Point> a_points = 
	{ info.GetAgents(TeamType::A)[0].GetPosition(), info.GetAgents(TeamType::A)[1].GetPosition() };

	Array<Point> b_points = 
	{ info.GetAgents(TeamType::B)[0].GetPosition(), info.GetAgents(TeamType::B)[1].GetPosition() };

	json += String(U"\"AgentPosA\" : " + pos_to_json(a_points[0], a_points[1]));
	json += String(U"\"AgentPosB\" : " + pos_to_json(b_points[0], b_points[1]));
	json += String(U"\"TotalPointA\" : " + Format(info.GetField().GetTotalPoints[0]) + U",\r\n");
	json += String(U"\"TotalPointB\" : " + Format(info.GetField().GetTotalPoints[1]) + U",\r\n");
	json += String(U"\"RemainingTurn\" : " + Format(info.GetField().GetTurn()) + U"\r\n }\r\n");
}
