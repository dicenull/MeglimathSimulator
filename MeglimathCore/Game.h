#pragma once
#include<Siv3D.hpp>
#include "Drawer.h"
#include "GameInfo.h"
#include "DrawingInfo.h"
#include"GameLogic/GameLogic.h"
class Game
{
private:
	GameLogic _gamelogic;
	HashTable<TeamType, Think> _think_table;

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
	Array<TeamLogic>& getTeamLogics();
	/// <summary>
	/// ゲーム情報を取得する
	/// </summary>
	/// <returns>フィールドとエージェントの情報</returns>
	GameInfo GetGameInfo() const;

	/// <summary>
	/// ターン数を取得する
	/// </summary>
	/// <returns>現在のターン</returns>
	int GetTurn() const;

	Field GetField() const;

	HashTable<TeamType, Think> GetThinks() const;

	/// <summary>
	/// ゲームを次のターンに進める
	/// </summary>
	void NextTurn(Think team_a, Think team_b);

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

public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="team_a">チームAの情報</param>
	/// <param name="team_b">チームBの情報</param>
	Game();


	/// <summary>
	/// jsonからゲームを初期化する
	/// </summary>
	/// <param name="path">jsonファイルへのパス</param>
	Game(const String path);

	virtual ~Game();
};
