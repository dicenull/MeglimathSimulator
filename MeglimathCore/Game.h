#pragma once
#include "Drawer.h"
#include "Field.h"
#include "Team.h"
#include "DrawingInfo.h"

class Game
{
private:
	Field _field;
	Team _teams[2];

	/// <summary>
	/// チームごとのすべてのエージェントの行動リスト
	/// </summary>
	HashTable<TeamType, Think> _thinks;

private:
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
	/// <summary>
	/// ゲーム情報を取得する
	/// </summary>
	/// <returns>フィールドとエージェントの情報</returns>
	GameInfo GetGameInfo() const;

	Field GetField() const;

	HashTable<TeamType, Think> GetThinks() const;

	/// <summary>
	/// ゲームを次のターンに進める
	/// </summary>
	void NextTurn();

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
	/// ゲームを更新する
	/// </summary>
	void Update();

public:
	Game(const String path);

	virtual ~Game();
};
