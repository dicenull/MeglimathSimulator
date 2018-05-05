#include "GameInfo.h"

Field GameInfo::GetField() const
{
	return _field;
}

Array<Agent> GameInfo::GetAgents(TeamType type) const
{
	return _agents.find(type)->second;
}

GameInfo::GameInfo(Field field, HashTable<TeamType, Array<Agent>> agents)
{
	_field = field;
	_agents = agents;
}

GameInfo::~GameInfo()
{
}
