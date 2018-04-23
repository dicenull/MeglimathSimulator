#pragma once
#include "Team.h"

class TyokushinTeam : public Team
{
public:
	virtual Think NextThink(GameInfo info) override
	{
		return Think({ Step{ Action::Move, Direction::Up }, Step{ Action::Move, Direction::Up } });
	}

};

