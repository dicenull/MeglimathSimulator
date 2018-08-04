#include "GameLogic.h"
#include<random>

std::unordered_map<TeamType, std::vector<Agent>> GameLogic::GetAgentMap() const
{
	std::unordered_map<TeamType, std::vector<Agent>> agents;
	agents[TeamType::A] = _teamlogics[0].GetAgents();
	agents[TeamType::B] = _teamlogics[1].GetAgents();

	return agents;
}
std::vector<Agent> GameLogic::GetAgents() const
{
	std::vector<Agent> ret{ _teamlogics[0].GetAgents() };
	auto && other = _teamlogics[1].GetAgents();
	ret.push_back(other[0]);
	ret.push_back(other[1]);
	return ret;
}
std::vector<TeamLogic>& GameLogic::getTeamLogics()
{
	return _teamlogics;
}
void GameLogic::initAgentsPos()
{
	_Size size = _field.GetCells().size();

	initAgentsPos(_Point<int>(std::rand() / (double)INT_MAX*(size.x - 2) / 2, std::rand() / (double)INT_MAX*(size.x - 2) / 2));
}

void GameLogic::initAgentsPos(_Point<> init_pos)
{
	_Size size = _field.GetCells().size();

	size -= _Point<int>(1, 1);
	_Point<> agent_pos[] =
	{
		init_pos,
		_Point<int>(size.x - init_pos.x, init_pos.y),
		_Point<int>(init_pos.x, size.y - init_pos.y),
		size - init_pos
	};

	// エージェントの初期位置のタイルを塗る
	_field.PaintCell(agent_pos[0], TeamType::A);
	_field.PaintCell(agent_pos[1], TeamType::A);

	_field.PaintCell(agent_pos[2], TeamType::B);
	_field.PaintCell(agent_pos[3], TeamType::B);

	_teamlogics[0].InitAgentsPos(agent_pos[0], agent_pos[1]);
	_teamlogics[1].InitAgentsPos(agent_pos[2], agent_pos[3]);
}


void GameLogic::InitalizeFromJson(const std::string json)
{
	rapidjson::Document document;
	document.Parse(json.data());

	_field = { json };
	if (document.HasMember("InitPos")) {
		initAgentsPos(_Point<int>{ document["InitPos"].GetString() });
	}
	else {
		initAgentsPos();
	}
	_turn = document["Turn"].GetInt();
}


int GameLogic::GetTurn() const
{
	return _turn;
}

void GameLogic::NextTurn(const std::unordered_map<TeamType, Think> &_thinks)
{
	if (_turn <= 0)
	{
		return;
	}
	auto agents_map = GetAgentMap();
	auto agents = GetAgents();

	// シミュレーション
	std::vector<std::pair<_Point<>, std::pair<Direction, TeamType>>> move_point_arr;
	std::vector<_Point<>> remove_points;
	for (TeamType team : {TeamType::A, TeamType::B})
	{
		for (int i = 0; i < 2; i++)
		{
			Direction dir = _thinks.at(team).steps[i].direction;
			// エージェントを動かしたい方向に動かした場合の座標
			_Point<int> pos = agents_map[team][i].GetPosition()+Transform::DirToDelta(dir);

			// エージェントが動作する座標を追加
			switch (_thinks.at(team).steps[i].action)
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
		auto team = pos_map.second.second;
		TileType our_tile = Transform::ToTile(team);
		TileType their_tile = Transform::GetInverseType(our_tile);

		// その座標に進むエージェントが一人であるか
		if (std::count_if(move_point_arr.cbegin(), move_point_arr.cend(), [pos](std::pair<_Point<>, std::pair<Direction, TeamType>> itr) {return itr.first == pos; }) != 1)
		{
			continue;
		}

		// その座標にエージェントがすでにいるか
		if (std::count_if(agents.cbegin(), agents.cend(), [pos](Agent agent) { return agent.GetPosition() == pos; }) != 0)
		{
			continue;
		}

		// その座標がフィールド内か
		if (_field.IsInField(pos) == false)
		{
			continue;
		}

		// その座標が相手のタイルか
		if (_field.GetCells()[pos.y][pos.x].GetTile() == their_tile)
		{
			continue;
		}

		auto dir = pos_map.second.first;

		// 進んだセルを塗る
		_field.PaintCell(pos, team);

		// 元の座標に戻す
		pos -= Transform::DirToDelta(dir);

		// エージェントを動かす
		_teamlogics[static_cast<int>(team)].MoveAgent(pos, dir);
	}

	// タイルを取る行動を実行
	for (auto & remove_point : remove_points)
	{
		if (std::count_if(remove_points.cbegin(), remove_points.cend(), [remove_point](auto p) {return p == remove_point; }) == 1 && _field.IsInField(remove_point))
		{
			_field.RemoveTile(remove_point);
		}
	}

	// ターンを進める
	_turn--;

	// チームの得点を更新
	_field.UpdatePoint();
}

bool GameLogic::IsThinkAble(TeamType team, Think think)const
{
	auto agents_map = GetAgentMap();
	auto agents = GetAgents();
	TileType our_tile = Transform::ToTile(team);
	TileType their_tile = Transform::GetInverseType(our_tile);
	int i = 0;
	for (auto step : think.steps) {
		Direction dir = step.direction;
		if ( step.action == Action::Move ) {
			// エージェントを動かしたい方向に動かした場合の座標
			_Point pos = agents_map[team][i].GetPosition() + Transform::DirToDelta(dir);
			if ( _field.IsInField(pos)
				&& _field.GetCells()[pos.y][pos.x].GetTile() != their_tile ) {
			} else {
				return false;
			}
		} else if ( step.action == Action::RemoveTile ) {
			// エージェントを動かしたい方向に動かした場合の座標
			_Point pos = agents_map[team][i].GetPosition() + Transform::DirToDelta(dir);
			if ( _field.IsInField(pos)
				&& _field.GetCells()[pos.y][pos.x].GetTile() != TileType::None ) {
			} else {
				return false;
			}
		}
		i++;
	}
	return true;
}

bool GameLogic::GetGameEnd()
{
	return GetTurn() == 0;
}

int GameLogic::GetWinner()
{
	if (GetTurn() != 0)return -1;
	auto p = _field.GetTotalPoints();
	constexpr int A = ( int )TeamType::A;
	constexpr int B = ( int )TeamType::B;
	if (p[A] > p[B]) {
		return A;
	}
	else if (p[B] > p[A]) {
		return B;
	}
	else {
		return -1;
	}
}

Field GameLogic::GetField() const
{
	return _field;
}

GameLogic::GameLogic() :_teamlogics({ TeamLogic(),TeamLogic() })
{

}

GameLogic::GameLogic(int turn) : _turn(turn), _teamlogics({ TeamLogic(),TeamLogic() })
{
}

GameLogic::GameLogic(int turn, _Size size) : _turn(turn), _teamlogics({ TeamLogic(),TeamLogic() }),_field(size){}

GameLogic::~GameLogic()
{
}
