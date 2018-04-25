#pragma once
#include "Team.h"
class KeyBoardTeam :
	public Team
{
public:
	Think NextThink(GameInfo info) override
	{
		Think think;
		for (int i : step(2))
		{
			if (!_next_steps[i].has_value())
			{
				return Think{ Step{Action::Stop}, Step{Action::Stop} };
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

private:
	/// <summary>
	/// キー入力がされた時の行動を格納しておく
	/// </summary>
	Optional<Step> _next_steps[2];

	/// <summary>
	/// すべての行動が選択済みであるかを判定する
	/// </summary>
	bool _is_ready = false;

private:
	/// <summary>
	/// キー入力によって行動する方向を決める。入力が得られなかった場合のためにOptionalとした
	/// </summary>
	/// <returns>キー入力によって決められた方向</returns>
	Optional<Direction> DecideDirectionByKey();

public:
	void Update(const Field & field);

public:
	KeyBoardTeam(TeamType type);
	KeyBoardTeam(TeamType type, Agent agent1, Agent agent2);
	~KeyBoardTeam();
};

