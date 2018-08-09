#include "TeamLogic.h"

void TeamLogic::InitAgentsPos(_Point<> pos1, _Point<> pos2)
{
	_agents[0] = Agent(pos1);
	_agents[1] = Agent(pos2);
}

std::vector<Agent> TeamLogic::GetAgents() const
{
	return std::vector<Agent>({ _agents[0], _agents[1] });
}

void TeamLogic::MoveAgent(int idx, Direction dir)
{
	_agents[idx].Move(dir);
}

void TeamLogic::MoveAgent(_Point<> pos, Direction dir)
{
	for (auto & agent : _agents)
	{
		if (agent.GetPosition() == pos)
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
	_agents[0] = agent1;
	_agents[1] = agent2;

	_type = type;
}

TeamLogic::TeamLogic(const TeamLogic &tl) :_type(tl.getTeamType()), _agents{ {tl._agents[0].GetPosition()},{tl._agents[1].GetPosition() } }
{}
