#pragma once
#include"Think.h"
#include "Field.h"
#include"TeamLogic.h"
#include<unordered_map>
#include<vector>
class GameLogic
{
private:
	Field _field;
	int _turn;
	std::vector<TeamLogic> _teamlogics;

public:

	/// <summary>
	/// エージェントをランダムに初期化する
	/// </summary>
	void initAgentsPos();

	/// <summary>
	/// エージェントの初期化処理を行う
	/// </summary>
	/// <param name="init_pos">エージェントの初期座標のもとになる左上の座標</param>
	void initAgentsPos(_Point<> init_pos);

public:
	std::vector<TeamLogic>& getTeamLogics();
	/// <summary>
	/// jsonからゲームを初期化する
	/// </summary>
	/// <param name="path">json</param>
	void InitalizeFromJson(const std::string json);

	int GetTurn() const;

	Field GetField() const;

	/// <summary>
	/// ゲームの状態を更新する
	/// すべてのエージェント情報を取得する
	/// </summary>
	/// <returns>すべてのエージェント情報リスト</returns>
	std::vector<Agent> GetAgents() const;

	/// <summary>
	/// チームごとのエージェントの情報を取得する
	/// </summary>
	/// <returns>チームごとのエージェント情報</returns>
	std::unordered_map<TeamType, std::vector<Agent>> GetAgentMap() const;

	/// <summary>
	/// ゲームを次のターンに進める
	/// </summary>
	void NextTurn(std::unordered_map<TeamType, Think> &_thinks);

	bool GetGameEnd();
	int GetWinner();

public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	GameLogic();
	GameLogic(int turn);

	virtual ~GameLogic();
};
