#pragma once
#include <Siv3D.hpp>
#include "Team.h"

class  RandomTeam : public Team
{
public:
	Think NextThink(GameInfo info) override
	{
		Action action = static_cast<Action>(Random(0, 1));
		Direction dir = static_cast<Direction>(Random(0, 7));
		Step step{ action, dir};

		return Think{ step, step };
	}

public:
	RandomTeam(TeamType type);
	RandomTeam(TeamType type, Agent agent1, Agent agent2);
};