#pragma once
#include "GameLogic/Field.h"
#include"GameLogic/GameLogic.h"
#include<Siv3D.hpp>

class GameInfo
{
private:
	Field _field;
	HashTable<TeamType, Array<Agent>> _agents;
	int _turn;

public:
	const Field& GetField() const;
	HashTable<TeamType, Array<Agent>> GetAllAgent() const;
	Array<Agent> GetAgents(TeamType type) const;
	int GetTurn() const;

	GameLogic GetGameLogic()const;
public:
	GameInfo();
	GameInfo(std::string json);
	GameInfo(Field field, int turn, HashTable<TeamType, Array<Agent>> agents);
	~GameInfo();
};

