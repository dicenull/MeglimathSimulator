#pragma once
#include <map>
#include "Field.h"

class GameInfo
{
private:
	Field _field;
	std::map<TeamType, Array<Agent>> _agents;

public:
	Field GetField();
	Array<Agent> GetAgents(TeamType type);

public:
	GameInfo(Field field, std::map<TeamType, Array<Agent>> agents);
	~GameInfo();
};

