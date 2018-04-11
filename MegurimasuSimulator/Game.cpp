#include "Game.h"

int Game::getAreaPoint(TileType tile)
{
	auto cells = _field.GetCells();
	_status = Grid<bool>(cells.size() + Point(2, 2), true);

	dfsAreaPoint(Point(0, 0), tile);

	int area_point = 0;
	for (int i : step(_status.width()))
	{
		for (int k : step(_status.height()))
		{
			if (_status[k][i])
			{
				area_point += cells[k - 1][i - 1].GetPoint();
			}
		}
	}
	
	return area_point;
}

void Game::dfsAreaPoint(Point pos, TileType tile)
{
	auto cells = _field.GetCells();

	// 範囲外なら終了
	if (pos.x < 0 || pos.x >= _status.width()
		|| pos.y < 0 || pos.y >= _status.height())
	{
		return;
	}

	_status[pos.y][pos.x] = false;
	if (pos.x == 0 || pos.x == cells.width() ||
		pos.y == 0 || pos.y == cells.height())
	{
		// 端は探索のみ行う
	}
	else if(cells[pos.y - 1][pos.x - 1].GetTile() == tile)
	{
		// 調査中のタイルが置かれていたら終了
		return;
	}
	
	Point delta[] = { Point(0, 1), Point(1, 0), Point(0, -1), Point(-1, 0) };

	// 四方へ探索する
	for (int i : step(4))
	{
		dfsAreaPoint(pos + delta[i], tile);
	}
}

GameInfo Game::getGameInfo() const
{
	return GameInfo(_field, getAgentMap());
}

std::map<TeamType, Array<Agent>> Game::getAgentMap() const
{
	std::map<TeamType, Array<Agent>> agents;
	agents[TeamType::A] = _teams[0]->GetAgents();
	agents[TeamType::B] = _teams[1]->GetAgents();

	return agents;
}

Array<Agent> Game::getAgents() const
{
	return _teams[0]->GetAgents().append(_teams[1]->GetAgents());
}

void Game::Update()
{
	std::map<TeamType, Think> thinks;
	
	GameInfo info = getGameInfo();
	auto agents_map = getAgentMap();
	auto agents = getAgents();

	thinks[TeamType::A] = _teams[0]->NextThink(info);
	thinks[TeamType::B] = _teams[1]->NextThink(info);

	// シミュレーション
	Array<std::pair<Point, std::pair<Direction, TeamType>>> move_point_arr;
	Array<Point> remove_points;
	for (TeamType team : {TeamType::A, TeamType::B})
	{
		for (int i = 0; i < 2; i++)
		{
			Direction dir = thinks[team].agents[i].direction;
			// エージェントを動かしたい方向に動かした場合の座標
			Point pos = agents_map[team][i].GetPosition().movedBy(Transform::DirToDelta(dir));

			// エージェントが動作する座標を追加
			switch (thinks[team].agents[i].action)
			{
			case Action::Move:
				move_point_arr.push_back(std::make_pair(pos, std::make_pair(dir, team)));
				break;
			case Action::RemoveTile:
				remove_points.push_back(pos);
				break;
			}
		}
	}

	// 衝突していないエージェントの行動のみ実行する
	for (auto & pos_map : move_point_arr)
	{
		auto pos = pos_map.first;

		// その座標に行くエージェントが一人、フィールド内
		// タイルが置かれていない、どのエージェントもいない
		if (move_point_arr.count_if([pos](std::pair<Point, std::pair<Direction, TeamType>> itr) {return itr.first == pos; }) == 1
			&& _field.IsInField(pos)
			&& _field.GetCells()[pos.y][pos.x].GetTile() == TileType::None
			&& agents.count_if([pos](Agent agent) { return agent.GetPosition() == pos; }) == 0)
		{
			auto dir = pos_map.second.first;
			auto team = pos_map.second.second;

			// 元の座標に戻す
			pos -= Transform::DirToDelta(dir);

			_teams[static_cast<int>(team)]->MoveAgent(pos, dir);
			_field.PaintCell(pos, team);
		}
	}

	for (auto & remove_point : remove_points)
	{
		if (remove_points.count_if(Equal(remove_point)) == 1 && _field.IsInField(remove_point))
		{
			_field.RemoveTile(remove_point);
		}
	}
	
}

void Game::Draw() const
{
	_field_drawer.Draw(getGameInfo());
}

Game::Game(const Field &field, std::shared_ptr<Team> team_a, std::shared_ptr<Team> team_b)
{
	_field = field;
	_teams.append({ team_a, team_b });
}

Game::~Game()
{
}
