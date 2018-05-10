#pragma once
#include "WaitingForInputTeam.h"

class KeyBoardTeam :
	public WaitingForInputTeam
{
private:
	Array<Key> _operation_keys;

private:
	/// <summary>
	/// キー入力によって行動する方向を決める。入力が得られなかった場合のためにOptionalとした
	/// </summary>
	/// <returns>キー入力によって決められた方向</returns>
	Optional<Direction> decideDirectionByKey();

public:
	void Update(const Field & field);

public:
	/// <summary>
	/// 
	/// </summary>
	/// <param name="team"></param>
	/// <param name="operation_keys">Rightから反時計回りに方向キーを指定</param>
	KeyBoardTeam(TeamLogic &team, Array<Key> operation_keys);

	~KeyBoardTeam();
};

