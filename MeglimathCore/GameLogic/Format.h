#pragma once
#include "Action.h"

namespace Format
{
	constexpr Action ToAction(const char * action_str)
	{
		if (action_str == "Move")
		{
			return Action::Move;
		}

		if (action_str == "Remove")
		{
			return Action::RemoveTile;
		}

		if (action_str == "Stop")
		{
			return Action::Stop;
		}

		return Action::Stop;
	}

	constexpr Direction ToDirection(const char * dir_str)
	{
		if (dir_str == "Right")
		{
			return Direction::Right;
		}
		if (dir_str == "RightUp")
		{
			return Direction::RightUp;
		}
		if (dir_str == "RightDown")
		{
			return Direction::RightDown;
		}
		if (dir_str == "Up")
		{
			return Direction::Up;
		}
		if (dir_str == "LeftUp")
		{
			return Direction::LeftUp;
		}
		if (dir_str == "Left")
		{
			return Direction::Left;
		}
		if (dir_str == "LeftDown")
		{
			return Direction::LeftDown;
		}
		if (dir_str == "Down")
		{
			return Direction::Down;
		}
		if (dir_str == "RightDown")
		{
			return Direction::RightDown;
		}
		if (dir_str == "Stop")
		{
			return Direction::Stop;
		}
		
		return Direction::Stop;
	}
}