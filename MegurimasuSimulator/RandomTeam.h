#pragma once
#include <Siv3D.hpp>
#include "../MeglimathCore/Team.h"

class  RandomTeam : public Team
{
public:
	Think NextThink(GameInfo info) override
	{
		return Think
		{
			Action(Random(0,1)),Direction(Random(0,7)),
			Action(Random(0,1)),Direction(Random(0,7))
		};
	}

	bool IsReady() override
	{
		return true;
	}

	void Update(const Field & field) override
	{

	}

public:
	RandomTeam(TeamLogic & team);
};