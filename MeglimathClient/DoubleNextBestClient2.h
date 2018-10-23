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