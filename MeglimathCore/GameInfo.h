#pragma once
#include "GameLogic/Field.h"

#include "rapidjson\rapidjson.h"
#include "rapidjson\writer.h"

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

public:
	std::string CreateJson();
};

