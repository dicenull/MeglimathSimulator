#include"DoubleNextBestClient2.h"
void DoubleNextBestClient2::Explore(const GameInfo& info, const GameLogic& game, int depth, int s1, int s2)
{
	auto this_team = _type;
	auto other_team = Transform::GetInverseTeam(this_team);

	const auto all_step = Utility::AllStep();

	if (depth == 0)
	{
		auto eval_point_total = 0;		// eval_point_total は eval_points_next の総和, 次の一手の評価基準
		for (int e : eval_points_next)
			eval_point_total += e;

		if (!game.GetField().IsSameStateField(info.GetField()))		//自分のエージェント同士の衝突を検出
		{
			for (int it = 0; it < 2; it++)
			{
				if (eval_points[it] <= eval_point_total)
				{
					if (eval_points[it] != eval_point_total)
						candidates[it].clear();

					eval_points[it] = eval_point_total;
					candidates[it].push_back({ all_step[s1], all_step[s2] });
					break;
				}
			}
		}
	}
	else
	{
		for (int i = 0; i < all_step.size(); i++)
		{
			for (int k = 0; k < all_step.size(); k++)
			{
				auto next_game = game;
				std::unordered_map<TeamType, Think> thinks;
				thinks[this_team] = Think{ all_step[i],all_step[k] };
				thinks[other_team] = Think{ Step{Action::Stop,Direction::Stop},Step{Action::Stop,Direction::Stop} };
				next_game.NextTurn(thinks);
				auto points = next_game.GetField().GetTotalPoints();
				eval_points_next[depth - 1] = points[this_team] - points[other_team];

				if (depth == EXPLORE_DEPTH)
					Explore(info, next_game, depth - 1, i, k);		// 最初のみ 仮引数 s1, s2 を更新
				else
					Explore(info, next_game, depth - 1, s1, s2);
			}
		}
	}
}

void DoubleNextBestClient2::Update(GameInfo info)
{
	auto agents = info.GetAgents(_type);
	auto game = info.GetGameLogic();
	auto this_team = _type;
	auto other_team = Transform::GetInverseTeam(this_team);

	// 自分のチームのエージェント両方の行動が連続で失敗した数を数える
//要修正 条件 : [移動していない] -> [盤面に変更がない]
	if (!(pos_history[0] == agents[0].position)
		|| !(pos_history[1] == agents[1].position))
		double_stop_cnt = 0;
	else
		double_stop_cnt++;

	pos_history[0] = agents[0].position;
	pos_history[1] = agents[1].position;

	Think next_thinks[2] = {
		{
			Step{ Action(Random(0, 1)), Direction(Random(0, 7)) },
			Step{ Action(Random(0, 1)), Direction(Random(0, 7)) }
		},
		{
			Step{ Action(Random(0, 1)), Direction(Random(0, 7)) },
			Step{ Action(Random(0, 1)), Direction(Random(0, 7)) }
		}
	};		// next_thinks[n] は (n+1)番目に優先される行動

	for (int it = 0; it < 2; it++)
		eval_points[it] = -100000;

	for (auto c : candidates)
		c.clear();

	Explore(info, game, EXPLORE_DEPTH, 0, 0);

	for (int it = 0; it < 2; it++)
		if (candidates[it].count() != (size_t)0)
			next_thinks[it] = Sample(candidates[it]);

	// 劣勢判断用変数 p_diff
	auto p_diff = game.GetField().GetTotalPoints()[this_team] - game.GetField().GetTotalPoints()[other_team];

	// 劣勢時に両方のエージェントが足止めされている場合、妥協して２番目に良い手を打つ
	if (double_stop_cnt > DOUBLE_STOP_LIMIT_FORCE)
		_think =
	{
		Step{ Action(Random(0, 1)), Direction(Random(0, 7)) },
		Step{ Action(Random(0, 1)), Direction(Random(0, 7)) }
	};
	else if (double_stop_cnt > DOUBLE_STOP_LIMIT && p_diff < 0)
		_think = next_thinks[1];
	else
		_think = next_thinks[0];

	_is_ready = true;
}