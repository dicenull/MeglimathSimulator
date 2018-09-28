#include "Utility.h"

namespace Utility
{
	const std::array<Step, 16> Utility::AllStep()
	{
		std::array<Step, 16> all_step = {};

		int index = 0;
		for (auto act : { Action::Move, Action::RemoveTile })
		{
			for (auto i = 0; i < 8; i++)
			{
				auto dir = static_cast<Direction>(i);
				all_step[index] = { act, dir };

				index++;
			}
		}

		return all_step;
	}
}