#pragma once
#include "GameInfo.h"
#include "Think.h"
#include "TeamType.h"
#include "Agent.h"

class Team
{
protected:
	/// <summary>
	/// チームの合計ポイント
	/// </summary>
	int _total_point;

	/// <summary>
	/// チームに所属するエージェント
	/// </summary>
	Agent _agents[2];

	/// <summary>
	/// チームA,Bを判断するためのチームの種類
	/// </summary>
	TeamType _type;

public:
	/// <summary>
	/// エージェントの次の行動をゲーム情報を元に実装する
	/// </summary>
	/// <param name="info">公開されるゲーム情報</param>
	/// <returns>エージェントの次の行動</returns>
	virtual Think NextThink(GameInfo info) = 0;

	int GetTotalPoint() const;
	void SetTotalPoint(int total_point);

	/// <summary>
	/// エージェントの初期位置を初期化する
	/// </summary>
	/// <param name="pos1">エージェント1の初期座標</param>
	/// <param name="pos2">エージェント2の初期座標</param>
	void InitAgentsPos(Point pos1, Point pos2);

	Array<Agent> GetAgents() const;
	
	/// <summary>
	/// 指定のエージェントを動かす
	/// </summary>
	/// <param name="idx">エージェントの番号</param>
	/// <param name="dir">動かす方向</param>
	void MoveAgent(int idx, Direction dir);

	/// <summary>
	/// 指定座標にいるエージェントを動かす
	/// </summary>
	/// <param name="pos">エージェントのいる座標</param>
	/// <param name="dir">動かす方向</param>
	void MoveAgent(Point pos, Direction dir);

public:
	Team();

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="type">チームの種類</param>
	/// <param name="agent1">一人目のエージェント</param>
	/// <param name="agent2">二人目のエージェント</param>
	Team(TeamType type, Agent agent1, Agent agent2);
	virtual ~Team();
};

