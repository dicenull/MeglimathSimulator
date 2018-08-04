#pragma once
#include "Types.h"
#include "Action.h"

class Agent
{
private:
	/// <summary>
	/// 座標
	/// </summary>
	_Point<> _position;

public:
	/// <summary>
	/// 座標を取得
	/// </summary>
	/// <returns>現在の座標</returns>
	_Point<> GetPosition() const;

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
	Agent(_Point<> position);

	virtual ~Agent();
};

