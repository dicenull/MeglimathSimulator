#include "GameInfo.h"
#include "LogicUtil.h"

Field GameInfo::GetField() const
{
	return _field;
}

HashTable<TeamType, Array<Agent>> GameInfo::GetAllAgent() const
{
	HashTable<TeamType, Array<Agent>> agent_hash;
	agent_hash[TeamType::A].append(GetAgents(TeamType::A));
	agent_hash[TeamType::B].append(GetAgents(TeamType::B));
	
	return agent_hash;
}

Array<Agent> GameInfo::GetAgents(TeamType type) const
{
	return _agents.find(type)->second;
}

int GameInfo::GetTurn() const
{
	return _turn;
}

GameInfo::GameInfo()
{
}

GameInfo::GameInfo(std::string json)
{
	_field = { json };

	rapidjson::Document document;
	document.Parse(json.data());

	_turn = document["RemainingTurn"].GetInt();

	// 座標のstringデータを_Pointに変換する
	auto point_to_agent = [&](std::string str) {return Agent{ LogicUtil::toLogicPoint((Parse<Point>(Unicode::Widen(str)))) }; };

	auto agent_pos = document["AgentPosA"].GetArray();
	_agents[TeamType::A].push_back(point_to_agent(agent_pos[0].GetString()));
	_agents[TeamType::A].push_back(point_to_agent(agent_pos[1].GetString()));

	agent_pos = document["AgentPosB"].GetArray();
	_agents[TeamType::B].push_back(point_to_agent(agent_pos[0].GetString()));
	_agents[TeamType::B].push_back(point_to_agent(agent_pos[1].GetString()));

}

GameInfo::GameInfo(Field field, int turn, HashTable<TeamType, Array<Agent>> agents)
{
	_field = field;
	_agents = agents;
	_turn = turn;
}

GameInfo::~GameInfo()
{
}

std::string GameInfo::CreateJson()
{
	auto cells = _field.GetCells();
	Size size = { (cells.size().y + 1) / 2, (cells.size().x + 1) / 2 };

	rapidjson::StringBuffer buffer;
	rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);

	writer.StartObject();

	writer.Key("Size");
	Point s(cells.size().x,cells.size().y);
	writer.String(Format(s).narrow().data());

	writer.Key("Points");
	writer.StartArray();
	for (int y : step(size.y))
	{
		for (int x : step(size.x))
		{
			writer.Int(cells[y][x].GetPoint());
		}
	}
	writer.EndArray();

	writer.Key("Tiles");
	writer.StartArray();
	for (int y : step(cells.size().y))
	{
		std::string str;
		for (int x : step(cells.size().x))
		{
			str.push_back(Transform::ToChar(cells[y][x].GetTile()));
		}

		writer.String(str.data());
	}
	writer.EndArray();

	writer.Key("AgentPosA");
	writer.StartArray();
	for (int i : step(2))
	{
		Point pos{ _agents[TeamType::A][i].GetPosition().x,_agents[TeamType::A][i].GetPosition().y };
		writer.String(Format(pos).narrow().data());
	}
	writer.EndArray();

	writer.Key("AgentPosB");
	writer.StartArray();
	for (int i : step(2))
	{
		Point pos{ _agents[TeamType::B][i].GetPosition().x,_agents[TeamType::B][i].GetPosition().y };
		writer.String(Format(pos).narrow().data());
	}
	writer.EndArray();

	writer.Key("TotalPointA");
	writer.Int(_field.GetTotalPoints()[0]);

	writer.Key("TotalPointB");
	writer.Int(_field.GetTotalPoints()[1]);

	writer.Key("RemainingTurn");
	writer.Int(_turn);

	writer.EndObject();

	return buffer.GetString();
}
