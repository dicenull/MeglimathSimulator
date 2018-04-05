#pragma once
#include "Field.h"
#include "Team.h"

class Game
{
private:
	Field _field;
	Team _teams[2];

public:
	Game();
	virtual ~Game();
};

