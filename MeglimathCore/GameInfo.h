#pragma once
#include "GameLogic/Field.h"

#include "rapidjson\rapidjson.h"
#include "rapidjson\writer.h"

class GameInfo
{
private:
	Field _field;
	HashTable<TeamType, Array<Agent>> _agents;
	int _turn;

public:
	Field GetField() const;
	Array<Agent> GetAgents(TeamType type) const;
	int GetTurn() const;

public:
	GameInfo(Field field, int turn, HashTable<TeamType, Array<Agent>> agents);
	~GameInfo();

public:
	std::string CreateJson();
};

