#pragma once
#include "Field.h"
#include "DrawingInfo.h"
#include "Think.h"
#include <Siv3D.hpp>

class Drawer : public DrawingInfo
{
public:
	void DrawField(const Field & field) const;
	void DrawAgents(std::map<TeamType, Array<Agent>> agents) const;
	void DrawStat(const std::map<TeamType, Think> & thinks, int turn) const;

public:
	Drawer();
	~Drawer();

	void operator=(const Drawer& other);
};

