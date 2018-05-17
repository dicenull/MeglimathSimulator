#pragma once
#include "../MeglimathCore/GameLogic/Think.h"
#include "../MeglimathCore/GameInfo.h"

class Client
{
protected:
	TeamType _type;

public:
	/// <summary>
	/// エージェントの次の行動をゲーム情報を元に実装する
	/// </summary>
	/// <param name="info">公開されるゲーム情報</param>
	/// <returns>エージェントの次の行動</returns>
	virtual Think NextThink(GameInfo info) = 0;

	virtual void Update() = 0;

	bool IsReady();

protected:
	bool _is_ready = false;

public:
	Client();
	Client(TeamType type);
	~Client();
};

