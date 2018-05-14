#include "Think.h"
#include "Action.h"
#include "Format.h"

Think::Think(std::string json)
{
	rapidjson::Document document;
	document.Parse(json.data());

	auto actions = document["Action"].GetArray();
	auto directions = document["Direction"].GetArray();
	
	for (int i = 0; i < 2; i++)
	{
		steps[i] =
		{ Format::ToAction(actions[i].GetString()), Format::ToDirection(actions[i].GetString()) };
	}
}

Think::Think()
{
}

Think::~Think()
{
}
