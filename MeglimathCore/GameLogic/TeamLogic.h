#pragma once
#include "Types.h"
#include "Agent.h"
#include <array>
struct TeamLogic
{	
	// フィールド //
	/// <summary>
	/// チームA,Bを判断するためのチームの種類
	/// </summary>
	TeamType type;

	/// <summary>
	/// チームに所属するエージェント
	/// </summary>
	std::array<Agent,2> agents;

	// メソッド //
	/// <summary>
	/// エージェントの初期位置を初期化する
	/// </summary>
	/// <param name="pos1">エージェント1の初期座標</param>
	/// <param name="pos2">エージェント2の初期座標</param>
	void InitAgentsPos(_Point<> pos1, _Point<> pos2)
	{
		agents = { { { pos1 },{ pos2 } } };
	}

	/// <summary>
	/// 指定のエージェントを動かす
	/// </summary>
	/// <param name="idx">エージェントの番号</param>
	/// <param name="dir">動かす方向</param>
	void MoveAgent(int idx, Direction dir)
	{
		agents[idx].Move(dir);
	}

	/// <summary>
	/// 指定座標にいるエージェントを動かす
	/// </summary>
	/// <param name="pos">エージェントのいる座標</param>
	/// <param name="dir">動かす方向</param>
	void MoveAgent(_Point<> pos, Direction dir)
	{
		for (auto & agent : agents)
		{
			if (agent.position == pos)
			{
				agent.Move(dir);
				return;
			}
		}
	}

};

