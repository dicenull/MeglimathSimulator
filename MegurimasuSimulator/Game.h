#pragma once
#include "Drawer.h"
#include "Field.h"
#include "Team.h"

class Game
{
private:
	Field _field;
	Drawer _drawer;
	Array<std::shared_ptr<Team>> _teams;
	int _turn;

private:
	GameInfo getGameInfo() const;
	std::map<TeamType, Array<Agent>> getAgentMap() const;
	Array<Agent> getAgents() const;

	void initAgents();
	void initAgents(Point init_pos);

public:
	void InitalizeFromJson(const String path);

	int GetTurn() const;

	void Update();
	void Draw() const;

public:
	Game(std::shared_ptr<Team> team_a, std::shared_ptr<Team> team_b);
	virtual ~Game();
};
