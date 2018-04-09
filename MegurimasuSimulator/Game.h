#pragma once
#include "FieldDrawer.h"
#include "Field.h"
#include "Team.h"


class Game
{
private:
	Field _field;
	FieldDrawer _field_drawer;
	Team _teams[2];

public:
	void Update();
	void Draw() const;

public:
	Game(const Field &field, Team team_a, Team team_b);
	virtual ~Game();
};
