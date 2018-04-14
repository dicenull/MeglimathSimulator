#pragma once
#include "Field.h"
#include "DrawingInfo.h"
#include <Siv3D.hpp>

class Drawer : public DrawingInfo
{
public:
	void DrawField(const Field & field) const;
	void DrawAgents(std::map<TeamType, Array<Agent>> agents) const;

public:
	Drawer();
	~Drawer();

	void operator=(const Drawer& other);
};

