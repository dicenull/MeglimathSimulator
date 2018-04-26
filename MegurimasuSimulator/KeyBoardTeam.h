#pragma once
#include "Team.h"
class KeyBoardTeam :
	public Team
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

private:
	/// <summary>
	/// キー入力がされた時の行動を格納しておく
	/// </summary>
	Optional<Step> _next_steps[2];

	/// <summary>
	/// すべての行動が選択済みであるかを判定する
	/// </summary>
	bool _is_ready = false;

	Array<Key> _operation_keys;

private:
	/// <summary>
	/// キー入力によって行動する方向を決める。入力が得られなかった場合のためにOptionalとした
	/// </summary>
	/// <returns>キー入力によって決められた方向</returns>
	Optional<Direction> DecideDirectionByKey();

public:
	void Update(const Field & field);

public:
	/// <summary>
	/// 
	/// </summary>
	/// <param name="type"></param>
	/// <param name="operation_keys">Rightから反時計回りに方向キーを指定</param>
	KeyBoardTeam(TeamType type, Array<Key> operation_keys);

	/// <summary>
	/// 
	/// </summary>
	/// <param name="type"></param>
	/// <param name="agent1"></param>
	/// <param name="agent2"></param>
	/// <param name="operation_keys">Rightから反時計回りに方向キーを指定</param>
	KeyBoardTeam(TeamType type, Agent agent1, Agent agent2, Array<Key> operation_keys);
	~KeyBoardTeam();
};

