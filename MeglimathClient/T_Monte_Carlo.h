#pragma once
#include <Siv3D.hpp>
#include "Client.h"
#include "../MeglimathCore/GameLogic/Think.h"

namespace TMC {
	/// <summary>
/// TeamTypeをTileTypeに変換する関数
/// </summary>
	TileType TeamtoTile(TeamType t);
}

class T_Monte_Carlo : public Client {
private:

	/// <summary>
	/// 動く先の候補を探す関数
	/// </summary>
	/// <param name="movelist">これまで動いたマス</param>
	/// <param name="agent">動作を決める対象のエージェント</param>
	/// <param name="field">フィールド情報</param>
	/// <returns>動く先</returns>
	int decideMove(Array<int> *movelist, _Point<> preP, Field field);

	/// <summary>
	/// クイックソート
	/// </summary>
	/// <param name="target">ソート対象</param>
	/// <param name="left">ソート対象の配列の一番左の要素</param>
	/// <param name="right">ソート対象の一番右の要素</param>
	void sort(Array <std::pair<Array<int>, int> > *target, int left, int right);



public:
	String Name() override
	{
		return U"T MonteCarlo";
	}

	void Initialize() override {
		_is_ready = false;
	}

	void Update(const GameInfo& info) override {

		if (info.GetTurn() == 0) {
			return;
		}

		//エージェント1、2のルートとそのルートによって得られるタイルポイントをセットで複数入れておく変数
		Array< std::pair<Array<int>, int> > agent1;
		Array< std::pair<Array<int>, int> > agent2;


		std::pair<Array<int>, int> agenttemp1;

		agenttemp1.second = 0;

		Field tem = info.GetField();
		Array<Agent> agents = info.GetAgents(type);
		int temppoint;
		_Point<> preP;

		while (1) {
			//エージェントの移動先をランダムに20手計算、結果を配列にpush
			preP = agents[0].position;
			if (info.GetTurn() >= 10) {
				for (int i = 0; i < 10; i++) {
					temppoint = decideMove(&(agenttemp1.first), preP, tem);

					agenttemp1.second += temppoint;

					preP += Transform::DirToDelta(Direction(agenttemp1.first[i]));
				}
			}
			else {
				for (int i = 0; i < info.GetTurn(); i++) {
					temppoint = decideMove(&(agenttemp1.first), preP, tem);

					agenttemp1.second += temppoint;

					preP += Transform::DirToDelta(Direction(agenttemp1.first[i]));
				}
			}

			agent1.push_back(agenttemp1);
			agenttemp1.first.clear();
			agenttemp1.second = 0;


			//計算結果が2通り以上になったらタイルポイント順でソート開始
			if (agent1.size() > 2) {
				sort(&agent1, 0, agent1.size() - 1);
				//タイルポイントが一番高いルートと、2番目に高いルートの一番最初の移動先が同じか1マスだけズレていたら、一番タイルポイントが高いルートの最初の移動先を最終的な移動先とする
				if (agent1[agent1.size() - 1].first[0] == agent1[agent1.size() - 2].first[0] || agent1[agent1.size() - 1].first[0] == (agent1[agent1.size() - 2].first[0]) + 1 || agent1[agent1.size() - 1].first[0] == (agent1[agent1.size() - 2].first[0]) - 1
					|| agent1.size() > 100) {
					break;
				}
			}

		}

		while (1) {
			preP = agents[1].position;
			if (info.GetTurn() >= 10) {
				for (int i = 0; i < 10; i++) {
					temppoint = decideMove(&(agenttemp1.first), preP, tem);
					agenttemp1.second += temppoint;
					preP += Transform::DirToDelta(Direction(agenttemp1.first[i]));
				}
			}
			else {
				for (int i = 0; i < info.GetTurn(); i++) {
					temppoint = decideMove(&(agenttemp1.first), preP, tem);
					agenttemp1.second += temppoint;
					preP += Transform::DirToDelta(Direction(agenttemp1.first[i]));
				}
			}


			agent2.push_back(agenttemp1);
			agenttemp1.first.clear();
			agenttemp1.second = 0;


			if (agent2.size() > 2) {
				sort(&agent2, 0, agent2.size() - 1);
				if (agent2[agent2.size() - 1].first[0] == agent2[agent2.size() - 2].first[0] || agent2[agent2.size() - 1].first[0] == (agent2[agent2.size() - 2].first[0]) + 1 || agent2[agent2.size() - 1].first[0] == (agent2[agent2.size() - 2].first[0]) - 1
					|| agent2.size() > 100) {
					break;
				}
			}

		}

		_think = Think{ tem.DecideStepByDirection(agents[0].position, Direction(agent1[agent1.size() - 1].first[0])), tem.DecideStepByDirection(agents[1].position, Direction(agent2[agent2.size() - 1].first[0])) };
		_is_ready = true;
	}

public:
	T_Monte_Carlo(TeamType type);
	~T_Monte_Carlo();
};
