#pragma once
#include "Drawer.h"
#include "Field.h"
#include "Team.h"
#include "DrawingInfo.h"

class Game
{
private:
	Field _field;
	Drawer _drawer;
	int _turn;

	/// <summary>
	/// 2チームの情報
	/// </summary>
	Array<std::shared_ptr<Team>> _teams;

	/// <summary>
	/// チームごとのすべてのエージェントの行動リスト
	/// </summary>
	std::map<TeamType, Think> _thinks;

private:
	/// <summary>
	/// ゲーム情報を取得する
	/// </summary>
	/// <returns>フィールドとエージェントの情報</returns>
	GameInfo getGameInfo() const;

	/// <summary>
	/// チームごとのエージェントの情報を取得する
	/// </summary>
	/// <returns>チームごとのエージェント情報</returns>
	std::map<TeamType, Array<Agent>> getAgentMap() const;

	/// <summary>
	/// すべてのエージェント情報を取得する
	/// </summary>
	/// <returns>すべてのエージェント情報リスト</returns>
	Array<Agent> getAgents() const;

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
	/// jsonからゲームを初期化する
	/// </summary>
	/// <param name="path">jsonファイルへのパス</param>
	void InitalizeFromJson(const String path);

	bool IsReady();

	/// <summary>
	/// ターン数を取得する
	/// </summary>
	/// <returns>現在のターン</returns>
	int GetTurn() const;

	/// <summary>
	/// ゲームを次のターンに進める
	/// </summary>
	void NextTurn();

	/// <summary>
	/// ゲームの状態を更新する
	/// </summary>
	void Update();

	/// <summary>
	/// ゲームを描画する
	/// </summary>
	void Draw() const;

public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="team_a">チームAの情報</param>
	/// <param name="team_b">チームBの情報</param>
	Game(std::shared_ptr<Team> team_a, std::shared_ptr<Team> team_b);

	virtual ~Game();
};
