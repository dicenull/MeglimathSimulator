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

	// TODO: フィールド外にはみ出していないか判定
	// シミュレーション
	Array<Point> move_points;
	Array<Point> remove_points;
	for (TeamType team : {TeamType::A, TeamType::B})
	{
		for (int i = 0; i < 2; i++)
		{
			// エージェントを動かしたい方向に動かした場合の座標
			Point pos = agents[team][i].GetPosition().movedBy(Transform::DirToDelta(thinks[team].agents[i].direction));

			switch (thinks[team].agents[i].action)
			{
			case Action::Move:
				move_points.push_back(pos);
				break;
			case Action::RemoveTile:
				remove_points.push_back(pos);
				break;
			}
		}
	}

	// 動作
	for (TeamType team : {TeamType::A, TeamType::B})
	{
		for (int i = 0; i < 2; i++)
		{
			Direction dir = thinks[team].agents[i].direction;
			// エージェントを動かしたい方向に動かした場合の座標
			Point pos = agents[team][i].GetPosition().movedBy(Transform::DirToDelta(dir));

			switch (thinks[team].agents[i].action)
			{
			case Action::Move:
				if (move_points.count_if(Equal(pos)) == 1)
				{
					_teams[static_cast<int>(team)].MoveAgent(i, dir);
					_field.PaintCell(pos, team);
				}
				break;
			case Action::RemoveTile:
				if (remove_points.count_if(Equal(pos)) == 1)
				{
					_field.RemoveTile(pos);
				}
				break;
			}
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
