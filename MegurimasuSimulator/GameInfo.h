#pragma once
#include <map>
#include "Field.h"

class GameInfo
{
private:
	Field _field;
	HashTable<TeamType, Array<Agent>> _agents;

public:
	Field GetField() const;
	Array<Agent> GetAgents(TeamType type) const;

public:
	GameInfo(Field field, HashTable<TeamType, Array<Agent>> agents);
	~GameInfo();
};

