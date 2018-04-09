#pragma once
#include "GameInfo.h"
#include "Think.h"
#include "TeamType.h"
#include "Agent.h"

class Team
{
private:
	int _total_point;
	Agent _agents[2];
	TeamType _type;

public:
	virtual Think NextThink(GameInfo info);

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

