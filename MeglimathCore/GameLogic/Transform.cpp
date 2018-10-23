#include "Transform.h"

const std::u32string Transform::ToString(Action action)
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

const std::u32string Transform::ToString(Direction dir)
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

const std::u32string Transform::ToString(Step step)
{
	return ToString(step.action) + U" " + ToString(step.direction);
}

const std::u32string Transform::ToString(TeamType team)
{
	switch (team)
	{
	case TeamType::Blue:
		return U"Blue";
	case TeamType::Red:
		return U"Red";
	}

	return U"";
}

const char Transform::ToChar(TileType tile)
{
	switch (tile)
	{
	case TileType::Blue:
		return 'a';
	case TileType::Red:
		return 'b';
	case TileType::None:
		return '-';
	}

}
