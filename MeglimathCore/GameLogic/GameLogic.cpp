#include "GameLogic.h"
#include<random>
#include <rapidjson/document.h>
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
	ret.assign(_teamlogics[1].GetAgents().cbegin(), _teamlogics[1].GetAgents().cend());
	return ret;
}
std::vector<TeamLogic>& GameLogic::getTeamLogics()
{
	return _teamlogics;
}
void GameLogic::initAgentsPos()
{
	_Size size = _field.GetCells().size();

	initAgentsPos(_Point<int>(std::rand()/(double)INT_MAX*(size.x - 2) / 2, std::rand() / (double)INT_MAX*(size.x - 2) / 2));
}

void GameLogic::initAgentsPos(_Point<> init_pos)
{
	_Size size = _field.GetCells().size();

	size -= _Point(1, 1);
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

	//JSONReader json(path);

	_field = Field(json);
	if (document.HasMember("InitPos")) {
		initAgentsPos(_Point{ document["InitPos"].GetString() });
	}
	else {
		initAgentsPos();
	}
	//if (json[U"InitPos"].isNull())
	//{
	//	initAgentsPos();
	//}
	//else
	//{
	//	initAgentsPos(json[U"InitPos"].get<Point>());
	//}
	_turn = document["Turn"].GetInt();
	//_turn = json[U"Turn"].get<int>();
}


int GameLogic::GetTurn() const
{
	return _turn;
}

void GameLogic::NextTurn(std::unordered_map<TeamType, Think> &_thinks)
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
			Direction dir = _thinks[team].steps[i].direction;
			// エージェントを動かしたい方向に動かした場合の座標
			_Point pos = agents_map[team][i].GetPosition()+Transform::DirToDelta(dir);

			// エージェントが動作する座標を追加
			switch (_thinks[team].steps[i].action)
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
		
		if (std::count_if(move_point_arr.cbegin(), move_point_arr.cend(), [pos](std::pair<_Point<>, std::pair<Direction, TeamType>> itr) {return itr.first == pos; }) == 1
			&& _field.IsInField(pos)
			&& _field.GetCells()[pos.y][pos.x].GetTile() == TileType::None
			&& std::count_if(agents.cbegin(), agents.cend(),[pos](Agent agent) { return agent.GetPosition() == pos; }) == 0)
		{
			auto dir = pos_map.second.first;
			auto team = pos_map.second.second;

			// 進んだセルを塗る
			_field.PaintCell(pos, team);

			// 元の座標に戻す
			pos -= Transform::DirToDelta(dir);

			// エージェントを動かす
			_teamlogics[static_cast<int>(team)].MoveAgent(pos, dir);
		}
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

Field GameLogic::GetField() const
{
	return _field;
}

GameLogic::GameLogic() :_teamlogics({ TeamLogic(),TeamLogic() })
{

}

GameLogic::~GameLogic()
{
}
