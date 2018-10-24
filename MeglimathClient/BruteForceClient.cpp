#include"BruteForceClient.h"
long long BruteForceClient::Explore(const GameInfo& info, const GameLogic& game, int depth)
{
	auto this_team = _type;
	auto other_team = Transform::GetInverseTeam(this_team);

	const auto all_step = Utility::AllStep();

	long long maxp = -1000000;
	for (int i = 0; i < all_step.size(); i++)
	{
		int p = 0;
		for (int k = 0; k < all_step.size(); k++)
		{
			long long pp = 100000000;
			for (int n = 0; n < all_step.size(); n++)
			{
				for (int m = 0; m < all_step.size(); m++)
				{
					auto next_game = game;
					std::unordered_map<TeamType, Think> thinks;
					thinks[this_team] = Think{ all_step[i],all_step[k] };
					thinks[other_team] = Think{ all_step[n],all_step[m] };
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

void BruteForceClient::Update(GameInfo info)
{
	auto agents = info.GetAgents(_type);
	auto game = info.GetGameLogic();
	auto point = game.GetField().GetTotalPoints();
	auto this_team = _type;
	auto other_team = Transform::GetInverseTeam(this_team);

	const auto all_step = Utility::AllStep();

	long long  maxp = -10000000;
	int maxi, maxk;
	for (int i = 0; i < all_step.size(); i++)
	{
		int p = 0;
		for (int k = 0; k < all_step.size(); k++)
		{
			long long pp = 0;
			for (int n = 0; n < all_step.size(); n++)
			{
				for (int m = 0; m < all_step.size(); m++)
				{
					auto next_game = game;
					std::unordered_map<TeamType, Think> thinks;
					thinks[this_team] = Think{ all_step[i],all_step[k] };
					thinks[other_team] = Think{ all_step[n],all_step[m] };
					next_game.NextTurn(thinks);
					//p = Explore(info, next_game, EXPLORE_DEPTH - 1);
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
	_think = { all_step[maxi],all_step[maxk] };

	_is_ready = true;
}