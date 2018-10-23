#pragma once
#include "../MeglimathCore/GameLogic/Think.h"
#include "../MeglimathCore/GameInfo.h"

class Client
{
public:
	TeamType type;
protected:
	Think _think;

public:
	Think GetNextThink();

public:
	/// <summary>
	/// 次の行動を決定する
	/// </summary>
	/// <param name="info">ゲーム情報</param>
	virtual void Update(const GameInfo& info) = 0;

	/// <summary>
	/// 次のターンに入る前の初期化処理
	/// </summary>
	virtual void Initialize();

	virtual void Draw();

	virtual String Name() = 0; 

	bool IsReady();

	virtual bool IsDraw();

protected:
	bool _is_ready = false;

public:
	Client();
	Client(TeamType type);
};

