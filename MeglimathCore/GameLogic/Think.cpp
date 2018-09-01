#include "Think.h"
#include "Format.h"

Think Think::makeThink(std::string json)
{
	Think ret;
	rapidjson::Document document;
	document.Parse(json.data());

	auto actions = document["Action"].GetArray();
	auto directions = document["Direction"].GetArray();

	for (int i = 0; i < 2; i++)
	{
		ret.steps[i].action = Format::ToAction(actions[i].GetString());
		ret.steps[i].direction = Format::ToDirection(directions[i].GetString());
	}
	return ret;
}