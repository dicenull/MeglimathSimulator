#include "Agent.h"
#include "Transform.h"


Point Agent::GetPosition() const
{
	return _position;
}

void Agent::Move(Direction dir)
{
	_position.moveBy(Transform::DirToDelta(dir));
}

Agent::Agent()
	: Agent(Point(0,0))
{}

Agent::Agent(Point position)
{
	_position = position;
}


Agent::~Agent()
{
}
