#pragma once
#include "Think.h"
#include <string>

namespace Format
{
	const Action ToAction(std::string action_str);

	const Direction ToDirection(std::string dir_str);
}