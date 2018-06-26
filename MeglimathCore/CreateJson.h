#pragma once
#include "../MeglimathCore/GameInfo.h"
#include "../MeglimathCore/GameLogic/Think.h"
#include "../MeglimathCore/GameLogic/Types.h"

#include <Siv3D.hpp>
#include <rapidjson\writer.h>
#include <string>

namespace Transform
{
	const std::string CreateJson(const GameInfo& info);
	const std::string CreateJson(const Think& think);
	const std::string CreateJson(const TeamType& type);
}
