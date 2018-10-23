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

	void Update(GameInfo info) override;
private:
	int double_stop_cnt = 0;		// 自分のエージェント両方の行動が連続して何回失敗したかをカウントする

	_Point<> pos_history[2] = { _Point<>(), _Point<>() };		// 前のターン自分のエージェントがどこにいたか

	long long  Explore(const GameInfo& info, const GameLogic& game, int depth);

	static constexpr int DOUBLE_STOP_LIMIT = 2;		// 自分のエージェント両方の行動の失敗が連続して DOUBLE_STOP_LIMIT 回を超えた場合かつ劣勢時に妥協した手を打つ
	static constexpr int DOUBLE_STOP_LIMIT_FORCE = 5;		// 自分のエージェント両方の行動の失敗が連続して DOUBLE_STOP_LIMIT_FORCE 回を超えた場合ランダムな手を打つ
	static constexpr int EXPLORE_DEPTH = 2;		// 探索の深さ = 何手先まで読むか
};