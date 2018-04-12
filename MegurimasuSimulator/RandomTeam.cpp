#include "RandomTeam.h"

RandomTeam::RandomTeam(TeamType type)
	:Team(type, Agent(), Agent())
{

}

RandomTeam::RandomTeam(TeamType type, Agent agent1, Agent agent2)
	:Team(type, agent1, agent2)
{}