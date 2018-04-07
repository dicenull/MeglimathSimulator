#include "GameInfo.h"

Field GameInfo::GetField()
{
	return _field;
}

Array<Agent> GameInfo::GetAgents(TeamType type)
{
	return _agents[type];
}

GameInfo::GameInfo(Field field, std::map<TeamType, Array<Agent>> agents)
{
	_field = field;
	_agents = agents;
}

GameInfo::~GameInfo()
{
}
