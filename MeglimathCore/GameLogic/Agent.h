#pragma once
#include "Types.h"
#include "Action.h"
#include "Transform.h"
struct Agent
{
	/// <summary>
	/// 座標
	/// </summary>
	_Point<> position;

	/// <summary>
	/// エージェントを動かす
	/// </summary>
	/// <param name="dir">動かす方向</param>
	void Move(Direction dir)
	{
		position += Transform::DirToDelta(dir);
	}

	Agent Moved(Direction dir)
	{
		Agent agent;
		agent.position = this->position;
		agent.Move(dir);
		return agent;
	}
};

