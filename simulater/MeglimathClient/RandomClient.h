#pragma once
#include "Client.h"
class RandomClient :
	public Client
{
public:
	Think NextThink(GameInfo info) override
	{
		return Think
		{
			{Action(Random(0,1)),Direction(Random(0,7))},
			{Action(Random(0,1)),Direction(Random(0,7))}
		};
	}

	void Update() override
	{}

public:
	RandomClient(TeamType type);
	~RandomClient();
};

