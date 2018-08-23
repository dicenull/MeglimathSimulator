#pragma once
#include "Client.h"
class RandomClient :
	public Client
{
public:
	String Name() override
	{
		return U"Random";
	}

	void Update(GameInfo info) override
	{
		if (IsReady())
		{
			return;
		}

		_think =
		{
			{Action(Random(0,1)),Direction(Random(0,7))},
			{Action(Random(0,1)),Direction(Random(0,7))}
		};
		_is_ready = true;
	}

	void Initialize() override
	{
		_is_ready = false;
	}

public:
	RandomClient(TeamType type);
	~RandomClient();
};

