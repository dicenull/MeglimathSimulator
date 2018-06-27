#include "Think.h"
#include "Think.h"
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
		steps[i].action = Format::ToAction(actions[i].GetString());
		steps[i].direction = Format::ToDirection(directions[i].GetString());
	}
}

Think::Think(Step steps[2])
	:Think(steps[0], steps[1])
{
}

Think::Think(Step step1, Step step2)
{
	steps[0] = step1;
	steps[1] = step2;
}

Think::Think()
{
}

Think::~Think()
{
}
