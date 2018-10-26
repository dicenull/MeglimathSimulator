
#include "GetEssentialStep.h"
#include "DoubleBruteForceClient.h"

long long DoubleBruteForceClient::Explore(const GameInfo& info, const GameLogic& game, int depth)
{
	auto field = info.GetField();
	auto this_team = type;
	auto other_team = Transform::GetInverseTeam(this_team);
	auto agents = info.GetAgents(this_team);
	auto other_agents = info.GetAgents(other_team);

	const auto all_step = Utility::AllStep();

	long long maxp = -1000000;
	for (Step i : GetEssentialStep(field, this_team, agents[0].position, EXPLORE_DEPTH))
	{
		int p = 0;
		for (Step k : GetEssentialStep(field, this_team, agents[1].position, EXPLORE_DEPTH))
		{
			long long pp = 100000000;
			for (Step n : GetEssentialStep(field, other_team, other_agents[0].position, EXPLORE_DEPTH))
			{
				for (Step m : GetEssentialStep(field, other_team, other_agents[1].position, EXPLORE_DEPTH))
				{
					auto next_game = game;
					std::unordered_map<TeamType, Think> thinks;
					thinks[this_team] = Think{ i,k };
					thinks[other_team] = Think{ n,m };
					next_game.NextTurn(thinks);
					if (depth == 1) {
						auto points = next_game.GetField().GetTotalPoints();
						p = points[this_team] - points[other_team];
					}
					else {
						p = Explore(info, next_game, depth - 1);
					}
					if (p < pp)pp = p;
				}
			}
			if (pp > maxp)maxp = pp;
		}
	}
	return maxp;
}

void DoubleBruteForceClient::Update(const GameInfo& info)
{
	auto game = info.GetGameLogic();
	auto field = info.GetField();
	auto point = game.GetField().GetTotalPoints();
	auto this_team = type;
	auto other_team = Transform::GetInverseTeam(this_team);
	auto agents = info.GetAgents(this_team);
	auto other_agents = info.GetAgents(other_team);

	const auto all_step = Utility::AllStep();

	long long  maxp = -10000000;
	Step maxi, maxk;
	for (Step i : GetEssentialStep(field, this_team, agents[0].position, EXPLORE_DEPTH))
	{
		int p = 0;
		for (Step k : GetEssentialStep(field, this_team, agents[1].position, EXPLORE_DEPTH))
		{
			long long pp = 0;
			for (Step n : GetEssentialStep(field, other_team, other_agents[0].position, EXPLORE_DEPTH))
			{
				for (Step m : GetEssentialStep(field, other_team, other_agents[1].position, EXPLORE_DEPTH))
				{
					auto next_game = game;
					std::unordered_map<TeamType, Think> thinks;
					thinks[this_team] = Think{ i,k };
					thinks[other_team] = Think{ n,m };
					next_game.NextTurn(thinks);
					auto points = next_game.GetField().GetTotalPoints();
					p = points[this_team] - points[other_team];
					if (p < pp)pp = p;
					pp += p;
				}
			}
			if (pp > maxp) {
				maxp = pp; maxi = i; maxk = k;
			}
		}
	}
	_think = { maxi,maxk };

	_is_ready = true;
}