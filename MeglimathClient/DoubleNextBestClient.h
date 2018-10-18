#pragma once
#include "Client.h"
#include "../MeglimathCore/Utility.h"
#include "../MeglimathCore/GameLogic/Think.h"

class DoubleNextBestClient :
	public Client
{
public:
	DoubleNextBestClient();
	DoubleNextBestClient(TeamType type);
	
	int double_stop_cnt = 0;
	const int DOUBLE_STOP_LIMIT = 2;
	const int DOUBLE_STOP_LIMIT_FORCE = 5;
	_Point<> pos_history[2] = { _Point<>(), _Point<>() };

	String Name() override
	{
		return U"DoubleNextBest";
	}

	void Initialize() override
	{
		_is_ready = false;
	}

	void Update(GameInfo info) override
	{
		auto agents = info.GetAgents(_type);		
		auto field = info.GetField();
		auto this_team = _type;
		auto other_team = Transform::GetInverseTeam(this_team);

		int best_eval_point = -100000;       //次の行動の最大の評価値
        int second_eval_point = -100000;        //次の行動の2番目の評価値
        int eval_point1;     // 一手目の評価値
		int eval_point2;     // 二手目の評価値

		if (!(pos_history[0] == agents[0].position)
			|| !(pos_history[1] == agents[1].position))
			double_stop_cnt = 0;
		else
			double_stop_cnt++;

		pos_history[0] = agents[0].position;
		pos_history[1] = agents[1].position;

		//Reseed(info.GetTurn());
		Think next_think =
		{
			Step{ Action(Random(0, 1)), Direction(Random(0, 7)) },
			Step{ Action(Random(0, 1)), Direction(Random(0, 7)) }
		};      //最善とされる行動

        Think next_think2 =
		{
			Step{ Action(Random(0, 1)), Direction(Random(0, 7)) },
			Step{ Action(Random(0, 1)), Direction(Random(0, 7)) }
		};      //２番目に最善とされる行動

		Array<Think> candidates, candidates2;
		candidates.clear();
		candidates2.clear();

		auto all_step = Utility::AllStep();

		for(int i = 0; i < all_step.size(); i++)
		{
			auto next_field1a = field.MakeFieldFromStep(this_team, agents[0], all_step[i]);

			for(int k = 0; k < all_step.size(); k++)
			{
				auto next_field1b = next_field1a.MakeFieldFromStep(this_team, agents[1], all_step[k]);
				
				auto next_points1 = next_field1b.GetTotalPoints();
				auto point_diff1 = next_points1[this_team] - next_points1[other_team];

				eval_point1 = point_diff1;
				eval_point2 = -100000;
                
                for(int m = 0; m < all_step.size(); m++)
                {
                    auto next_field2a = next_field1b.MakeFieldFromStep(this_team, agents[0], all_step[m]);

                    for(int p = 0; p < all_step.size(); p++)
                    {
                        auto next_field2b = next_field2a.MakeFieldFromStep(this_team, agents[1], all_step[p]);
				        
                        auto next_points2 = next_field2b.GetTotalPoints();
                        auto point_diff2 = next_points2[this_team] - next_points2[other_team];        
                        
						if (eval_point2 < point_diff2)
							eval_point2 = point_diff2;
                    }
                }
                
				auto eval_point_total = eval_point1 + eval_point2;
				
				if (!next_field1b.IsSameStateField(field))		//自分のエージェント同士の衝突を検出
				{
					if (best_eval_point <= eval_point_total)
					{
						if (best_eval_point != eval_point_total)
							candidates.clear();

						best_eval_point = eval_point_total;

						candidates.push_back({ all_step[i], all_step[k] });
					}
					else if (second_eval_point <= eval_point_total)
					{
						if (second_eval_point != eval_point_total)
							candidates2.clear();

						second_eval_point = eval_point_total;

						candidates2.push_back({ all_step[i], all_step[k] });
					}
				}
			}
		}

		if (candidates.count() != (size_t)0)
			next_think = Sample(candidates);
		if (candidates2.count() != (size_t)0)
			next_think2 = Sample(candidates2);

		auto points = field.GetTotalPoints();
		auto point_diff = points[this_team] - points[other_team];

		// 劣勢時に両方のエージェントが足止めされている場合、妥協して２番目に良い手を打つ
		if (double_stop_cnt > DOUBLE_STOP_LIMIT_FORCE)
			_think =
				{
					Step{ Action(Random(0, 1)), Direction(Random(0, 7)) },
					Step{ Action(Random(0, 1)), Direction(Random(0, 7)) }
				};
		else if (double_stop_cnt > DOUBLE_STOP_LIMIT && point_diff < 0)
			_think = next_think2;
		else
			_think = next_think;

		_is_ready = true;
	}
};