#pragma once
#include"Think.h"
#include "Field.h"
#include"TeamLogic.h"
class GameLogic
{
private:
	Field _field;
	int _turn;
	Array<TeamLogic> _teamlogics;

public:

	/// <summary>
	/// エージェントをランダムに初期化する
	/// </summary>
	void initAgentsPos();

	/// <summary>
	/// エージェントの初期化処理を行う
	/// </summary>
	/// <param name="init_pos">エージェントの初期座標のもとになる左上の座標</param>
	void initAgentsPos(Point init_pos);

public:
	Array<TeamLogic>& getTeamLogics();
	/// <summary>
	/// jsonからゲームを初期化する
	/// </summary>
	/// <param name="path">jsonファイルへのパス</param>
	void InitalizeFromJson(const String path);

	int GetTurn() const;

	Field GetField() const;

	/// <summary>
	/// ゲームの状態を更新する
	/// すべてのエージェント情報を取得する
	/// </summary>
	/// <returns>すべてのエージェント情報リスト</returns>
	Array<Agent> GetAgents() const;

	/// <summary>
	/// チームごとのエージェントの情報を取得する
	/// </summary>
	/// <returns>チームごとのエージェント情報</returns>
	HashTable<TeamType, Array<Agent>> GetAgentMap() const;

	/// <summary>
	/// ゲームを次のターンに進める
	/// </summary>
	void NextTurn(HashTable<TeamType, Think> &_thinks);


public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="team_a">チームAの情報</param>
	/// <param name="team_b">チームBの情報</param>
	GameLogic();

	virtual ~GameLogic();
};
