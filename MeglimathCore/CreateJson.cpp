#include "CreateJson.h"

const std::string Transform::CreateJson(const GameInfo& info)
{
	const Field & field = info.GetField();
	auto agents = info.GetAllAgent();
	int turn = info.GetTurn();

	const auto &cells = field.cells;
	Size size = { cells.size().x, cells.size().y };

	rapidjson::StringBuffer buffer;
	rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);

	writer.StartObject();

	writer.Key("Size");
	Point s(cells.size().x, cells.size().y);
	writer.String(Format(s).narrow().data());

	writer.Key("Points");
	writer.StartArray();
	for (size_t y : step(size.y))
	{
		for (size_t x : step(size.x))
		{
			writer.Int(cells[{x, y}].point);
		}
	}
	writer.EndArray();

	writer.Key("Tiles");
	writer.StartArray();
	for (auto y : step(cells.size().y))
	{
		std::string str;
		for (auto x : step(cells.size().x))
		{
			str.push_back(Transform::ToChar(cells[{x, y}].tile));
		}

		writer.String(str.data());
	}
	writer.EndArray();

	writer.Key("AgentPosA");
	writer.StartArray();
	for (int i : step(2))
	{
		Point pos{ agents[TeamType::Blue][i].position.x, agents[TeamType::Blue][i].position.y };
		writer.String(Format(pos).narrow().data());
	}
	writer.EndArray();

	writer.Key("AgentPosB");
	writer.StartArray();
	for (int i : step(2))
	{
		Point pos{ agents[TeamType::Red][i].position.x, agents[TeamType::Red][i].position.y };
		writer.String(Format(pos).narrow().data());
	}
	writer.EndArray();

	writer.Key("TotalPointA");
	writer.Int(field.GetTotalPoints()[0]);

	writer.Key("TotalPointB");
	writer.Int(field.GetTotalPoints()[1]);

	writer.Key("RemainingTurn");
	writer.Int(turn);

	writer.EndObject();

	return buffer.GetString();
}

const std::string Transform::CreateJson(const Think& think)
{
	rapidjson::StringBuffer buffer;
	rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);

	writer.StartObject();
	writer.Key("Action");
	writer.StartArray();
	writer.String(String(Transform::ToString(think.steps[0].action)).toUTF8().data());
	writer.String(String(Transform::ToString(think.steps[1].action)).toUTF8().data());
	writer.EndArray();

	writer.Key("Direction");
	writer.StartArray();
	writer.String(String(Transform::ToString(think.steps[0].direction)).toUTF8().data());
	writer.String(String(Transform::ToString(think.steps[1].direction)).toUTF8().data());
	writer.EndArray();
	writer.EndObject();

	return buffer.GetString();
}

const std::string Transform::CreateJson(const TeamType & type)
{
	rapidjson::StringBuffer buffer;
	rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
	writer.StartObject();
	writer.Key("TeamType");

	std::string type_obj;
	switch (type)
	{
	case TeamType::Blue:
		type_obj = "A";
		break;
	case TeamType::Red:
		type_obj = "B";
		break;
	}
	writer.String(type_obj.data());
	writer.EndObject();

	return buffer.GetString();
}
