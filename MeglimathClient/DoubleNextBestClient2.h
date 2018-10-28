#pragma once

#include "Client.h"
#include "../MeglimathCore/Utility.h"
#include "../MeglimathCore/GameLogic/Think.h"

class DoubleNextBestClient2 :
	public Client
{
public:
	DoubleNextBestClient2() {}
	DoubleNextBestClient2(TeamType type):Client(type) {}

	String Name() override
	{
		return U"DoubleNextBest2";
	}

	void Initialize() override
	{
		_is_ready = false;
	}

	void Update(const GameInfo& info) override {}
private:
	void Explore(const GameInfo& info, const GameLogic& game, int depth, int s1, int s2);
};