#pragma once
#include "ControlTower.h"
#include "Agent.h"

class Team
{
private:
	int _total_point;
	ControlTower _control_tower;
	Agent _agents[2];

public:
	Team();
	virtual ~Team();
};

