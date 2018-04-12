#pragma once
#include "FieldDrawer.h"
#include "Field.h"
#include "Team.h"

// TODO: —Ìˆæƒ|ƒCƒ“ƒg‚Ìˆ—
class Game
{
private:
	Field _field;
	FieldDrawer _field_drawer;
	Array<std::shared_ptr<Team>> _teams;

	Array<Point> _arrives;

private:
	bool dfsAreaPoint(Point pos, TileType tile);

	GameInfo getGameInfo() const;
	std::map<TeamType, Array<Agent>> getAgentMap() const;
	Array<Agent> getAgents() const;

public:
	void InitAgents();

	void Update();
	void Draw() const;

public:
	Game(const Field &field, std::shared_ptr<Team> team_a, std::shared_ptr<Team> team_b);
	virtual ~Game();
};
