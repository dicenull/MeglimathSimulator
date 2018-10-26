#pragma once

#include "Client.h"
#include "../MeglimathCore/Utility.h"
#include "../MeglimathCore/GameLogic/Think.h"

class DoubleBruteForceClient :
	public Client
{
public:
	DoubleBruteForceClient() {}
	DoubleBruteForceClient(TeamType type) :Client(type) {}

	String Name() override
	{
		return U"DoubleBruteForce";
	}

	void Initialize() override
	{
		_is_ready = false;
	}

	void Update(const GameInfo& info) override;
private:
	long long  Explore(const GameInfo& info, const GameLogic& game, int depth);

	static constexpr int EXPLORE_DEPTH = 2;		// íTçıÇÃê[Ç≥ = âΩéËêÊÇ‹Ç≈ì«ÇﬁÇ©
};