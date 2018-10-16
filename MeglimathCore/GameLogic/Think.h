#pragma once
#include<array>
#include <rapidjson\document.h>
#include <string>

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

struct Step
{
	Action action;
	Direction direction;
};

/// <summary>
/// エージェント二人分の行動
/// </summary>
struct Think
{
	std::array<Step,2> steps;

	static Think makeThink(std::string json);
};