#pragma once
#include "GameInfo.h"
#include "Think.h"
#include "Types.h"
#include "Agent.h"
#include "TeamLogic.h"
class Team
{
protected:
	TeamLogic & _team;
public:
	/// <summary>
	/// エージェントの次の行動をゲーム情報を元に実装する
	/// </summary>
	/// <param name="info">公開されるゲーム情報</param>
	/// <returns>エージェントの次の行動</returns>
	virtual Think NextThink(GameInfo info) = 0;

	/// <summary>
	/// チームのThinkデータの格納が完了しているか
	/// </summary>
	virtual bool IsReady() = 0;

	virtual void Update(const Field & field) = 0;
	
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	Team(TeamLogic & team);
	virtual ~Team();
};

