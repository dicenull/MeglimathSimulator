#include "Game.h"


GameInfo Game::getGameInfo() const
{
	return GameInfo(_field, getAgentMap());
}

std::map<TeamType, Array<Agent>> Game::getAgentMap() const
{
	std::map<TeamType, Array<Agent>> agents;
	agents[TeamType::A] = _teams[0].GetAgents();
	agents[TeamType::B] = _teams[1].GetAgents();

	return agents;
}

void Game::Update()
{
	std::map<TeamType, Think> thinks;
	
	GameInfo info = getGameInfo();
	auto agents = getAgentMap();

	thinks[TeamType::A] = _teams[0].NextThink(info);
	thinks[TeamType::B] = _teams[1].NextThink(info);

	// シミュレーション
	std::map<Point, Array<std::pair<Direction, TeamType>>> move_point_map;
	Array<Point> remove_points;
	for (TeamType team : {TeamType::A, TeamType::B})
	{
		for (int i = 0; i < 2; i++)
		{
			Direction dir = thinks[team].agents[i].direction;
			// エージェントを動かしたい方向に動かした場合の座標
			Point pos = agents[team][i].GetPosition().movedBy(Transform::DirToDelta(dir));

			// エージェントが動作する座標を追加
			switch (thinks[team].agents[i].action)
			{
			case Action::Move:
				move_point_map[pos].push_back(std::make_pair(dir, team));
				break;
			case Action::RemoveTile:
				remove_points.push_back(pos);
				break;
			}
		}
	}

	// 衝突していないエージェントの行動のみ実行する
	for (auto & pos_map : move_point_map)
	{
		auto pos = pos_map.first;

		if (pos_map.second.count() == 1 && _field.IsInField(pos))
		{	
			auto dir = pos_map.second[0].first;
			auto team = pos_map.second[0].second;

			_teams[static_cast<int>(team)].MoveAgent(pos, dir);
			_field.PaintCell(pos, team);
		}
	}

	for (auto & remove_point : remove_points)
	{
		if (remove_points.count_if(Equal(fgetpos)) == 1 && _field.IsInField(remove_point))
		{
			_field.RemoveTile(remove_point);
		}
	}
	
}

void Game::Draw() const
{
	_field_drawer.Draw(getGameInfo());
}

Game::Game(const Field &field, Team team_a, Team team_b)
{
	_field = field;
	_teams[0] = team_a;
	_teams[1] = team_b;
}

Game::~Game()
{
}
