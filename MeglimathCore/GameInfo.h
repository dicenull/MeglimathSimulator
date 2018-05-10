#pragma once
#include <map>
#include "GameLogic/Field.h"
#include <cereal/cereal.hpp>
//#include <cereal/archives/json.hpp>

class GameInfo
{
private:
	Field _field;
	HashTable<TeamType, Array<Agent>> _agents;

public:
	Field GetField() const;
	Array<Agent> GetAgents(TeamType type) const;

public:
	GameInfo(Field field, HashTable<TeamType, Array<Agent>> agents);
	~GameInfo();

public:
	template<class Archive>
	void serialize(Archive &archive)
	{
		std::vector<Point> pos_a = 
		{ 
			_agents[TeamType::A][0].GetPosition(),
			_agents[TeamType::A][1].GetPosition()
		};

		std::vector<Point> pos_b =
		{
			_agents[TeamType::B][0].GetPosition(),
			_agents[TeamType::B][1].GetPosition()
		};


		archive(
			CEREAL_NVP(_field.GetCells().size()),
			CEREAL_NVP(_field.GetCells()),
			cereal::make_nvp("AgentPosA", pos_a),
			cereal::make_nvp("AgentPosB", pos_b),
			cereal::make_nvp("TotalPointA", _field.GetTotalPoints()[0]),
			cereal::make_nvp("TotalPointB", _field.GetTotalPoints()[1]),
			cereal::make_nvp("RemainingTurn", _field.GetTurn())
			);
	}
};

