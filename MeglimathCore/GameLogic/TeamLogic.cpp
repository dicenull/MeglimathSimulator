#include "TeamLogic.h"

void TeamLogic::InitAgentsPos(_Point<> pos1, _Point<> pos2)
{
	agents[0] = Agent{ pos1 };
	agents[1] = Agent{ pos2 };
}

const std::array<Agent, 2>& TeamLogic::GetAgents() const
{
	return agents;
}

void TeamLogic::MoveAgent(int idx, Direction dir)
{
	agents[idx].Move(dir);
}

void TeamLogic::MoveAgent(_Point<> pos, Direction dir)
{
	for (auto & agent : agents)
	{
		if (agent.position == pos)
		{
			agent.Move(dir);
			return;
		}
	}
}

TeamType TeamLogic::getTeamType()const
{
	return _type;
}

TeamLogic::TeamLogic()
	:TeamLogic(TeamType::A, Agent(), Agent())
{}

TeamLogic::TeamLogic(TeamType type, Agent agent1, Agent agent2)
{
	agents[0] = agent1;
	agents[1] = agent2;

	_type = type;
}

TeamLogic::TeamLogic(const TeamLogic &tl) :
	_type(tl.getTeamType()), 
	agents{tl.GetAgents()}
{}
