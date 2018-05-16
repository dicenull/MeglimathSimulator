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
