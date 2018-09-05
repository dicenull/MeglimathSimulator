#pragma once
#include "Action.h"
#include<array>
#include <rapidjson\document.h>
#include <string>

/// <summary>
/// エージェント二人分の行動
/// </summary>
struct Think
{
	std::array<Step,2> steps;

	static Think makeThink(std::string json);
};