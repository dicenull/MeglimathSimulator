#include "GameInfo.h"
#include "LogicUtil.h"

const Field& GameInfo::GetField() const
{
	return _field;
}

HashTable<TeamType, Array<Agent>> GameInfo::GetAllAgent() const
{
	HashTable<TeamType, Array<Agent>> agent_hash;
	agent_hash[TeamType::Blue].append(GetAgents(TeamType::Blue));
	agent_hash[TeamType::Red].append(GetAgents(TeamType::Red));
	
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

GameLogic GameInfo::GetGameLogic() const
{
	GameLogic ret;
	auto& ATeam = _agents.find(TeamType::Blue)->second;
	auto& BTeam = _agents.find(TeamType::Red)->second;
	ret.InitializeVariable(_turn, _field,
	{
		TeamLogic{{ATeam[0],ATeam[1]}},
		TeamLogic{{BTeam[0],BTeam[1]}}
	});
	return ret;
}

GameInfo::GameInfo()
{
}

GameInfo::GameInfo(std::string json)
{
	_field =Field::makeFieldFromJson(json);

	rapidjson::Document document;
	document.Parse(json.data());

	_turn = document["RemainingTurn"].GetInt();

	// 座標のstringデータを_Pointに変換する
	auto point_to_agent = [&](std::string str) {return Agent{ LogicUtil::toLogicPoint((Parse<Point>(Unicode::Widen(str)))) }; };

	auto agent_pos = document["AgentPosA"].GetArray();
	_agents[TeamType::Blue].push_back(point_to_agent(agent_pos[0].GetString()));
	_agents[TeamType::Blue].push_back(point_to_agent(agent_pos[1].GetString()));

	agent_pos = document["AgentPosB"].GetArray();
	_agents[TeamType::Red].push_back(point_to_agent(agent_pos[0].GetString()));
	_agents[TeamType::Red].push_back(point_to_agent(agent_pos[1].GetString()));

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
