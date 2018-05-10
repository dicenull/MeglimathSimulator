#pragma once
#include "WaitingForInputTeam.h"

class GamePadTeam : public WaitingForInputTeam
{
private:
	Optional<Direction> decideDirectionByGamePad();

public:
	void Update(const Field & field);

public:
	GamePadTeam(TeamLogic &team);
	~GamePadTeam();
};

