#pragma once

#include "Client.h"
#include "../MeglimathCore/Utility.h"
#include "../MeglimathCore/GameLogic/Think.h"

class BruteForceClient :
	public Client
{
public:
	BruteForceClient() {}
	BruteForceClient(TeamType type) :Client(type) {}

	String Name() override
	{
		return U"BruteForce";
	}

	void Initialize() override
	{
		_is_ready = false;
	}

	void Update(GameInfo info) override;
private:
	long long  Explore(const GameInfo& info, const GameLogic& game, int depth);
	
	static constexpr int EXPLORE_DEPTH = 1;		// íTçıÇÃê[Ç≥ = âΩéËêÊÇ‹Ç≈ì«ÇﬁÇ©
};