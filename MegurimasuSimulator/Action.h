#pragma once

/// <summary>
/// エージェントの動作
/// </summary>
enum class Action
{
	Move, RemoveTile, Stop
};

/// <summary>
/// 動作の方向
/// </summary>
enum class Direction
{
	Right,
	RightUp,
	Up,
	LeftUp,
	Left,
	LeftDown,
	Down,
	RightDown,
	Stop
};

struct Actions
{
	Action action;
	Direction direction;
};