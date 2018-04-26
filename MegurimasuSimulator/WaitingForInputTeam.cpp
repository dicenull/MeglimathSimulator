#include "WaitingForInputTeam.h"



WaitingForInputTeam::WaitingForInputTeam(TeamType type)
	:Team(type, Agent(), Agent()) {}

WaitingForInputTeam::WaitingForInputTeam(TeamType type, Agent agent1, Agent agent2)
	: Team(type, agent1, agent2) {}

WaitingForInputTeam::WaitingForInputTeam()
{
}


WaitingForInputTeam::~WaitingForInputTeam()
{
}
