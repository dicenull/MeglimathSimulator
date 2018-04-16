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

private:
	GameInfo getGameInfo() const;
	std::map<TeamType, Array<Agent>> getAgentMap() const;
	Array<Agent> getAgents() const;

public:
	void InitAgents();
	void InitAgents(Point init_pos);

	void Update();
	void Draw() const;

public:
	Game(const Field &field, std::shared_ptr<Team> team_a, std::shared_ptr<Team> team_b);
	virtual ~Game();
};
