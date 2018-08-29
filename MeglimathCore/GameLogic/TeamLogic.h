#pragma once
#include "Types.h"
#include "Agent.h"
#include <array>
class TeamLogic
{
private:
	/// <summary>
	/// チームに所属するエージェント
	/// </summary>
	std::array<Agent,2> agents;

	/// <summary>
	/// チームA,Bを判断するためのチームの種類
	/// </summary>
	TeamType _type;

public:
	/// <summary>
	/// エージェントの初期位置を初期化する
	/// </summary>
	/// <param name="pos1">エージェント1の初期座標</param>
	/// <param name="pos2">エージェント2の初期座標</param>
	void InitAgentsPos(_Point<> pos1, _Point <>pos2);

	const std::array<Agent, 2>& GetAgents() const;

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
	void MoveAgent(_Point<> pos, Direction dir);
	TeamType getTeamType() const;
public:
	TeamLogic();

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="type">チームの種類</param>
	/// <param name="agent1">一人目のエージェント</param>
	/// <param name="agent2">二人目のエージェント</param>
	TeamLogic(TeamType type, Agent agent1, Agent agent2);

	//コピーコンストラクタ
	TeamLogic(const TeamLogic &tl);
};

