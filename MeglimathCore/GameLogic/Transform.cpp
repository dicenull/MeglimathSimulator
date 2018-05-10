#include "Transform.h"

const String Transform::ToString(Action action)
{
	switch (action)
	{
	case Action::Move:
		return U"Move";
	case Action::RemoveTile:
		return U"Remove";
	case Action::Stop:
		return U"Stop";
	}

	return U"";
}

const String Transform::ToString(Direction dir)
{
	switch (dir)
	{
	case Direction::Right:
		return U"Right";

	case Direction::RightUp:
		return U"RightUp";

	case Direction::Up:
		return U"Up";

	case Direction::LeftUp:
		return U"LeftUp";

	case Direction::Left:
		return U"Left";

	case Direction::LeftDown:
		return U"LeftDown";

	case Direction::Down:
		return U"Down";

	case Direction::RightDown:
		return U"RightDown";

	}
	return U"";
}

const String Transform::ToString(Step step)
{
	return ToString(step.action) + U" " + ToString(step.direction);
}

const String Transform::ToString(TeamType team)
{
	switch (team)
	{
	case TeamType::A:
		return U"Team A";
	case TeamType::B:
		return U"Team B";
	}

	return U"";
}
