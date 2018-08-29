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
const std::vector<TeamLogic>& GameLogic::getTeamLogics()const
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
	initAgentPos({ agent_pos[0], agent_pos[1], agent_pos[2], agent_pos[3] });
}

void GameLogic::initAgentsPos(_Point<> init_pos1, _Point<> init_pos2)
{
	_Size size = _field.GetCells().size();
	_Point<> top_left;
	if (init_pos1.x > size.x / 2)
	{
		top_left.x = init_pos1.x / 2;
	}
	else
	{
		top_left.x = init_pos1.x;
	}

	if (init_pos1.y > size.y / 2)
	{
		top_left.y = init_pos1.y / 2;
	}
	else
	{
		top_left.y = init_pos1.y;
	}

	bool init_map[][2] = { {false, false}, {false, false} };
	init_map[init_pos1.x / (size.x / 2)][init_pos1.y / (size.y / 2)] = true;
	init_map[init_pos2.x / (size.x / 2)][init_pos2.y / (size.y / 2)] = true;

	std::vector<_Point<>> other_init_pos;
	for (int i = 0; i < 2; i++)
	{
		for (int k = 0; k < 2; k++)
		{
			if (init_map[i][k] == false)
			{
				_Point<> pos;
				pos.x = i 
					? (int)(size.x - 1) - top_left.x
					: top_left.x;

				pos.y = k
					? (int)(size.y - 1) - top_left.y
					: top_left.y;

				other_init_pos.push_back(pos);
			}
		}
	}

	initAgentPos({ init_pos1, init_pos2, other_init_pos[0], other_init_pos[1] });
}

void GameLogic::initAgentPos(std::vector<_Point<>> init_pos)
{
	_field.PaintCell(init_pos[0], TeamType::A);
	_field.PaintCell(init_pos[1], TeamType::A);

	_field.PaintCell(init_pos[2], TeamType::B);
	_field.PaintCell(init_pos[3], TeamType::B);

	_teamlogics[0].InitAgentsPos(init_pos[0], init_pos[1]);
	_teamlogics[1].InitAgentsPos(init_pos[2], init_pos[3]);
}


void GameLogic::InitalizeFromJson(const std::string json)
{
	rapidjson::Document document;
	document.Parse(json.data());

	_field = { json };
	// TODO: 必要であれば二人分の初期位置を取得
	if (document.HasMember("InitPos")) {
		auto init_pos = document["InitPos"].GetArray();
		initAgentsPos(_Point<int>{ init_pos[0].GetString()}, _Point<int>{init_pos[1].GetString()});
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
	std::vector<_Point<>> stop_points;
	for (TeamType team : {TeamType::A, TeamType::B})
	{
		for (int i = 0; i < 2; i++)
		{
			Direction dir = _thinks.at(team).steps[i].direction;
			// エージェントを動かしたい方向に動かした場合の座標
			_Point<int> pos = agents_map[team][i].GetPosition();
			_Point<int> after_pos = pos+Transform::DirToDelta(dir);

			// エージェントが動作する座標を追加
			switch (_thinks.at(team).steps[i].action)
			{
			case Action::Move:
				move_point_arr.push_back(std::make_pair(after_pos, std::make_pair(dir, team)));
				break;
			case Action::RemoveTile:
				stop_points.push_back(pos);
				remove_points.push_back(after_pos);
				break;
			case Action::Stop:
				stop_points.push_back(pos);
				break;
			}
		}
	}

	// 衝突していないエージェントの行動のみ実行する
	for (auto i = 0;i < move_point_arr.size();i++)
	{
		auto & pos_map = move_point_arr[i];
		auto pos = pos_map.first;
		auto team = pos_map.second.second;
		auto dir = pos_map.second.first;
		TileType our_tile = Transform::ToTile(team);
		TileType their_tile = Transform::GetInverseType(our_tile);

		// その座標に進むエージェントが一人であること
		if (std::count_if(move_point_arr.cbegin(), move_point_arr.cend(), [pos](std::pair<_Point<>, std::pair<Direction, TeamType>> itr) {return itr.first == pos; }) != 1)
		{
			stop_points.push_back(pos - Transform::DirToDelta(dir));
			move_point_arr.erase(move_point_arr.begin() + i);

			for (int k = 0; k < move_point_arr.size(); k++)
			{
				auto other_pos = move_point_arr[k].first;
				auto other_dir = move_point_arr[k].second.first;

				if (pos == other_pos)
				{
					stop_points.push_back(other_pos - Transform::DirToDelta(other_dir));
					move_point_arr.erase(move_point_arr.begin() + k);
				}
			}

			i = -1;
			continue;
		}

		// その座標のタイルを除去するエージェントがいないこと
		// その座標にとどまるエージェントがいないこと
		// その座標がフィールド内であること
		// その座標に相手のタイルがないこと
		if (   std::count_if(remove_points.cbegin(), remove_points.cend(), [pos](auto p) {return p == pos; }) > 0
			|| std::count_if(stop_points.cbegin(), stop_points.cend(), [pos](auto p) {return p == pos; }) > 0
			|| _field.IsInField(pos) == false
			|| _field.GetCells()[pos.y][pos.x].tile == their_tile)
		{
			// 停留に変更する
			stop_points.push_back(pos - Transform::DirToDelta(dir));
			move_point_arr.erase(move_point_arr.begin() + i);

			// 初めからシミュレーションを再開
			i = -1;
			continue;
		}
	}

	for (auto pos_map : move_point_arr)
	{
		auto pos = pos_map.first;
		auto team = pos_map.second.second;
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
		// そのタイルを除去するエージェントが一人であること
		// その座標に移動するエージェントがいないこと
		// 除去するタイルがフィールド内にあること
		// 除去するマスにとどまるエージェントがいないこと
		if (std::count_if(remove_points.cbegin(), remove_points.cend(), [remove_point](auto p) {return p == remove_point; }) != 1
			|| std::count_if(move_point_arr.cbegin(), move_point_arr.cend(), [remove_point](std::pair<_Point<>, std::pair<Direction, TeamType>> itr) {return itr.first == remove_point; }) > 0
			|| !_field.IsInField(remove_point)
			|| std::count_if(stop_points.cbegin(), stop_points.cend(), [remove_point](auto p) {return p == remove_point; }) > 0)
		{
			continue;
		}

		_field.RemoveTile(remove_point);
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
				&& _field.GetCells()[pos.y][pos.x].tile != their_tile ) {
			} else {
				return false;
			}
		} else if ( step.action == Action::RemoveTile ) {
			// エージェントを動かしたい方向に動かした場合の座標
			_Point pos = agents_map[team][i].GetPosition() + Transform::DirToDelta(dir);
			if ( _field.IsInField(pos)
				&& _field.GetCells()[pos.y][pos.x].tile != TileType::None ) {
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
	auto total_points = _field.GetTotalPoints();
	if (total_points[0] > total_points[1]) {
		return (int)TeamType::A;
	}
	else if (total_points[0] < total_points[1]) {
		return (int)TeamType::B;
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

GameLogic::GameLogic(const GameLogic & gamelogic):_turn(gamelogic.GetTurn()),_field(gamelogic.GetField()),_teamlogics(gamelogic.getTeamLogics())
{
}
GameLogic::GameLogic(const Field &field, int t, const std::vector<TeamLogic>& tl) : _field(field), _turn(t), _teamlogics(tl)
{
}
GameLogic::~GameLogic()
{
}
