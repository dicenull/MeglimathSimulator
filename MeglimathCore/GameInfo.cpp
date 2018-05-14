#include "GameInfo.h"

Field GameInfo::GetField() const
{
	return _field;
}

Array<Agent> GameInfo::GetAgents(TeamType type) const
{
	return _agents.find(type)->second;
}

int GameInfo::GetTurn() const
{
	return _turn;
}

GameInfo::GameInfo(Field field, int turn, HashTable<TeamType, Array<Agent>> agents)
{
	_field = field;
	_agents = agents;
	_turn = turn;
}

GameInfo::~GameInfo()
{
}

using namespace rapidjson;

std::string GameInfo::CreateJson()
{
	auto cells = _field.GetCells();
	Size size = { (cells.size().y + 1) / 2, (cells.size().x + 1) / 2 };

	StringBuffer buffer;
	Writer<StringBuffer> writer(buffer);

	writer.StartObject();

	writer.Key("Size");
	Point s(cells.size().x,cells.size().y);
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
		Point pos{ _agents[TeamType::A][i].GetPosition().x,_agents[TeamType::A][i].GetPosition().y };
		writer.String(Format(pos).narrow().data());
	}
	writer.EndArray();

	writer.Key("AgentPosB");
	writer.StartArray();
	for (int i : step(2))
	{
		Point pos{ _agents[TeamType::B][i].GetPosition().x,_agents[TeamType::A][i].GetPosition().y };
		writer.String(Format(pos).narrow().data());
	}
	writer.EndArray();

	writer.Key("TotalPointA");
	writer.Int(_field.GetTotalPoints()[0]);

	writer.Key("TotalPointB");
	writer.Int(_field.GetTotalPoints()[1]);

	writer.Key("RemainingTurn");
	writer.Int(_turn);

	writer.EndObject();

	return buffer.GetString();
}
