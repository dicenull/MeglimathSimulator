#include "CreateJson.h"

const std::string Transform::CreateJson(const GameInfo& info)
{
	Field & field = info.GetField();
	auto agents = info.GetAllAgent();
	int turn = info.GetTurn();

	auto cells = field.GetCells();
	Size size = { (cells.size().y + 1) / 2, (cells.size().x + 1) / 2 };

	rapidjson::StringBuffer buffer;
	rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);

	writer.StartObject();

	writer.Key("Size");
	Point s(cells.size().x, cells.size().y);
	writer.String(Format(s).narrow().data());

	writer.Key("Points");
	writer.StartArray();
	for (int y : step(size.y))
	{
		for (int x : step(size.x))
		{
			writer.Int(cells[y][x].GetPoint());
		}
	}
	writer.EndArray();

	writer.Key("Tiles");
	writer.StartArray();
	for (int y : step(cells.size().y))
	{
		std::string str;
		for (int x : step(cells.size().x))
		{
			str.push_back(Transform::ToChar(cells[y][x].GetTile()));
		}

		writer.String(str.data());
	}
	writer.EndArray();

	writer.Key("AgentPosA");
	writer.StartArray();
	for (int i : step(2))
	{
		Point pos{ agents[TeamType::A][i].GetPosition().x, agents[TeamType::A][i].GetPosition().y };
		writer.String(Format(pos).narrow().data());
	}
	writer.EndArray();

	writer.Key("AgentPosB");
	writer.StartArray();
	for (int i : step(2))
	{
		Point pos{ agents[TeamType::B][i].GetPosition().x, agents[TeamType::B][i].GetPosition().y };
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
	writer.Key("TeamType");
	writer.String(RandomBool() ? "A" : "B");
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
