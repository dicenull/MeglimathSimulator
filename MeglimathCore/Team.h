#pragma once
#include "GameInfo.h"
#include "Think.h"
#include "Types.h"
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

public:
	int GetTotalPoint() const;
	void SetTotalPoint(int total_point);

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
	/// <param name="agent1">一人目のエージェントの初期座標</param>
	/// <param name="agent2">二人目のエージェントの初期座標</param>
	Team(Point pos1, Point pos2);

	virtual ~Team();
};

