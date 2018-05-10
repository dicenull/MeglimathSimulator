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

public:
	/// <summary>
	/// 座標を取得
	/// </summary>
	/// <returns>現在の座標</returns>
	Point GetPosition() const;

	/// <summary>
	/// エージェントを動かす
	/// </summary>
	/// <param name="dir">動かす方向</param>
	void Move(Direction dir);

public:
	Agent();

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="position">初期座標</param>
	Agent(Point position);

	virtual ~Agent();
};

