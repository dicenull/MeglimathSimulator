#include "Agent.h"
#include "Transform.h"


_Point<> Agent::GetPosition() const
{
	return _position;
}

void Agent::Move(Direction dir)
{
	_position+=Transform::DirToDelta(dir);
}

Agent::Agent()
	: Agent(_Point<>(0,0))
{}

Agent::Agent(_Point<> position)
{
	_position = position;
}


Agent::~Agent()
{
}
