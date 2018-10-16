#pragma once
#include"Think.h"
#include "Field.h"
#include"TeamLogic.h"
#include<unordered_map>
#include<vector>
#include <rapidjson\document.h>

class GameLogic
{
private:
	int _turn = 60;
	Field _field;
	std::array<TeamLogic, 2> teams = {};

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

	void initAgentsPos(_Point<> init_pos1, _Point<> init_pos2);

	void initAgentPos(std::array<_Point<>,4> init_pos);

public:
	/// <summary>
	/// jsonからゲームを初期化する
	/// </summary>
	/// <param name="path">json</param>
	void InitalizeFromJson(const std::string json);

	void InitializeRandom(int turn, int height, int width);
	void InitializeVariable(int turn, const Field& field, const std::array<TeamLogic, 2>& teams);
	int GetTurn() const;
	std::array<TeamLogic, 2> GetTeams()const;
	const Field& GetField() const;

	/// <summary>
	/// ゲームの状態を更新する
	/// すべてのエージェント情報を取得する
	/// </summary>
	/// <returns>すべてのエージェント情報リスト</returns>
	std::vector<Agent> GetAgents() const;

	/// <summary>
	/// ゲームを次のターンに進める
	/// </summary>
	void NextTurn(const std::unordered_map<TeamType, Think> &_thinks);

	///<summary>
	///可能な行動か
	///</summary>
	bool IsThinkAble(TeamType team, Think think)const;

	bool GetGameEnd();
	int GetWinner();
};
