#pragma once
#include "Action.h"
#include<array>
/// <summary>
/// エージェント二人分の行動
/// </summary>
struct Think
{
	Think(){}
	Think(Step s1, Step s2) :steps{ s1,s2 } {}
	std::array<Step,2> steps;
};