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

	String Name() override
	{
		return U"DoubleNextBest";
	}

	void Update(const GameInfo& info) override
	{
		auto agents = info.GetAgents(type);		
		auto field = info.GetField();
		auto this_team = type;
		auto other_team = Transform::GetInverseTeam(this_team);

		int best_eval_point = -100000;       //次の行動の最大の評価値
        int second_eval_point = -100000;        //次の行動の2番目の評価値
        int eval_point1;     // 一手目の評価値
		int eval_point2;     // 二手目の評価値

		Reseed(info.GetTurn());
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

				if (best_eval_point <= eval_point_total)
				{
					best_eval_point = eval_point_total;
					next_think = { all_step[i], all_step[k] };
				}
                else if (second_eval_point <= eval_point_total)
                {
                    second_eval_point = eval_point_total;
					next_think2 = { all_step[i], all_step[k] };
                }
			}
		}	

		_think = next_think;
		_is_ready = true;
	}
};