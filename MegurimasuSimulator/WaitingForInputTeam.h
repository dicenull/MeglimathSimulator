#pragma once
#include <Siv3D.hpp>
#include "Team.h"

class WaitingForInputTeam : public Team
{
public:
	Think NextThink(GameInfo info) override
	{
		Think think = { Step{ Action::Stop }, Step{ Action::Stop } };
		for (int i : step(2))
		{
			if (!_next_steps[i].has_value())
			{
				return think;
			}

			think.steps[i] = _next_steps[i].value();
			// 次の更新のために初期化
			_next_steps[i].reset();
		}
		_is_ready = false;

		return think;
	}

	bool IsReady() override
	{
		return _is_ready;
	}

protected:
	/// <summary>
	/// キー入力がされた時の行動を格納しておく
	/// </summary>
	Optional<Step> _next_steps[2];

	/// <summary>
	/// すべての行動が選択済みであるかを判定する
	/// </summary>
	bool _is_ready = false;

public:
	WaitingForInputTeam(TeamLogic &team );
	virtual ~WaitingForInputTeam();
};

