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
	
	int double_stop_cnt = 0;		// 自分のエージェント両方の行動が連続して何回失敗したかをカウントする
	const int DOUBLE_STOP_LIMIT = 2;		// 自分のエージェント両方の行動の失敗が連続して DOUBLE_STOP_LIMIT 回を超えた場合かつ劣勢時に妥協した手を打つ
	// DOUBLE_STOP_LIMIT_FORCE は 今後消去する(無限にする)予定
	const int DOUBLE_STOP_LIMIT_FORCE = 5;		// 自分のエージェント両方の行動の失敗が連続して DOUBLE_STOP_LIMIT_FORCE 回を超えた場合ランダムな手を打つ
	static const int EXPLORE_DEPTH = 2;		// 探索の深さ = 何手先まで読むか

	_Point<> pos_history[2] = { _Point<>(), _Point<>() };		// 前のターン自分のエージェントがどこにいたか
	Array<Think> candidates[2];		// candidates[n] は (n+1)番目に優先される行動の候補リスト

	int eval_points[2] = { -100000, -100000 };		// eval_points[n] は (n+1)番目に優先される 1手後の行動 の評価値
	int eval_points_next[EXPLORE_DEPTH];		// eval_points_next[n] は (n+1)手後の行動 の評価値


	String Name() override
	{
		return U"DoubleNextBest";
	}

	/// <summary>
	/// 指定した方向に進んだときの座標の変化分を返す
	/// </summary>
	_Point<int> DirectionToDeltaPos(Direction dir)
	{
		switch (dir)
		{
		case Direction::Right:
			return _Point<int>(1, 0);
		case Direction::RightUp:
			return _Point<int>(1, -1);
		case Direction::Up:
			return _Point<int>(0, -1);
		case Direction::LeftUp:
			return _Point<int>(-1, -1);
		case Direction::Left:
			return _Point<int>(-1, 0);
		case Direction::LeftDown:
			return _Point<int>(-1, 1);
		case Direction::Down:
			return _Point<int>(0, 1);
		case Direction::RightDown:
			return _Point<int>(1, 1);
		default:
			return _Point<int>(0, 0);
		}
	}

	/// <summary>
	/// 指定したフィールド内の指定したマスからみて指定した方向にあるタイルの色
	/// </summary>
	TileType GetTeamFromNext(Field field, _Point<int> pos, Step step)
	{
		Direction dir = step.direction;
		_Point<int> pos_next = pos + DirectionToDeltaPos(dir);
		if (field.IsInField(pos_next))
			return field.cells[pos_next.y][pos_next.x].tile;
		else
			return TileType::None;		//ダミー
	}

	/// <summary>
	/// 指定したフィールド内の指定したマスからみて指定した方向にあるタイルの得点が負数かどうか
	/// </summary>
	bool GetIsNegativeFromNext(Field field, _Point<int> pos, Step step)
	{
		Direction dir = step.direction;
		_Point<int> pos_next = pos + DirectionToDeltaPos(dir);
		if (field.IsInField(pos_next))
			return field.cells[pos_next.y][pos_next.x].point >= 0;
		else
			return true;	//ダミー
	}

	/// <summary>
	/// フィールドの状態とエージェントの位置を見て、探索に有効と思われる Step を選択する。
	/// </summary>
	Array<Step> GetEssentialStep(Field field, TeamType team, _Point<int> pos)
	{
		Array<Step> essential_step = {};
		TileType thiss_tile = static_cast<TileType>(team);
		TileType others_tile = Transform::GetInverseTile(thiss_tile);

		for (auto act : { Action::Move, Action::RemoveTile })
		{
			for (auto i = 0; i < 8; i++)
			{
				auto dir = static_cast<Direction>(i);
				Step step = { act, dir };		// stepの生成を省く実装をすることでより高速化できるかもしれない
				TileType tile = GetTeamFromNext(field, pos, step);
				bool is_negative = GetIsNegativeFromNext(field, pos, step);

				if (tile == TileType::None && act == Action::RemoveTile)
					continue;
				if (tile == thiss_tile && !is_negative && act == Action::RemoveTile)
					continue;
				if (tile == others_tile && act == Action::Move)
					continue;

				essential_step.push_back({ act, dir });
			}
		}

		return essential_step;
	}

	Array<_Point<int>> GetNewPositionsFromSteps(Array<_Point<int>> positions, Array<Step> steps)
	{
		return { 
			positions[0] + DirectionToDeltaPos(steps[0].direction),
			positions[1] + DirectionToDeltaPos(steps[1].direction),
		};
	}

	/// <summary>
	/// depth手後に相手との得点差が最大になるように探索する再帰関数。
	/// </summary>
	/// <param name="info">現在のフィールドやエージェントの状態、チーム情報。再帰による値の変動なし</param>
	/// <param name="field">探索中のフィールドの状態。再帰による値の変動あり</param>
	/// <param name="depth">現在の状態から探索する残り手数。再帰による値の変動あり</param>
	/// <param name="positions">探索中の２人のエージェントの位置。再帰による値の変動あり</param>
	/// <param name="s1">探索１手目のエージェントの動き。再帰の最初の呼び出しでのみ値の変動あり</param>
	void Explore(GameInfo info, Field field, int depth, Array<_Point<int>> positions, Array<Step> s1)
	{
		auto agents = info.GetAgents(type);
		auto original = info.GetField();
		auto this_team = type;
		auto other_team = Transform::GetInverseTeam(this_team);

		auto all_step = Utility::AllStep();

		if (depth == 0)		// n手の探索の後処理
		{
			auto eval_point_total = 0;		// eval_point_total は eval_points_next の総和, 次の一手の評価基準
			for (int e : eval_points_next)
				eval_point_total += e;

			if (!field.IsSameStateField(original))		// 自分のエージェント同士の衝突が検出されなければ最善手のリストを更新
			{
				for (int it = 0; it < 2; it++)
				{
					if (eval_points[it] <= eval_point_total)
					{
						if (eval_points[it] != eval_point_total)
							candidates[it].clear();

						eval_points[it] = eval_point_total;
						candidates[it].push_back({ s1[0],s1[1] });
						break;
					}
				}
			}
		}
		else
		{	
			// n手の探索
			for (auto step1 : GetEssentialStep(field, this_team, positions[0]))
			{
				auto next_field_a = field.MakeFieldFromStep(this_team, agents[0], step1);

				for (auto step2 : GetEssentialStep(field, this_team, positions[1]))
				{
					auto next_field_b = next_field_a.MakeFieldFromStep(this_team, agents[1], step2);

					eval_points_next[depth - 1] = next_field_b.GetTotalPoints()[this_team] - next_field_b.GetTotalPoints()[other_team];

					if (depth == EXPLORE_DEPTH)
						Explore(info, next_field_b, depth - 1, GetNewPositionsFromSteps(positions, { step1,step2 }), { step1,step2 });		// 最初のみ 仮引数 s1, s2 を更新
					else
						Explore(info, next_field_b, depth - 1, GetNewPositionsFromSteps(positions, { step1,step2 }), s1);
				}
			}
		}
	}

	void Update(const GameInfo& info) override
	{
		auto agents = info.GetAgents(type);		
		auto field = info.GetField();
		auto this_team = type;
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

		Explore(
			info,
			field,
			EXPLORE_DEPTH,
			{ agents[0].position,agents[1].position },
			{ {Action::Stop,Direction::Stop},{Action::Stop,Direction::Stop} }
		);		// 探索本体に現在地とダミーのStepを2つ渡す

		for (int it = 0; it < 2; it++)
			if (candidates[it].count() != (size_t)0)
				next_thinks[it] = Sample(candidates[it]);

		// 劣勢判断用変数 p_diff
		auto p_diff = field.GetTotalPoints()[this_team] - field.GetTotalPoints()[other_team];

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
};