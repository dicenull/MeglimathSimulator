#pragma once
#include <Siv3D.hpp>
#include "Action.h"

class Agent
{
private:
	/// <summary>
	/// 取得したタイルポイント
	/// </summary>
	int _tile_point;

	/// <summary>
	/// 座標
	/// </summary>
	Point _position;

	Actions _actions;

public:
	Agent();
	virtual ~Agent();
};

