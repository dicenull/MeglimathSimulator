#include "KeyBoardTeam.h"



Optional<Direction> KeyBoardTeam::DecideDirectionByKey()
{
	Optional<Direction> dir = none;

	// Rightから反時計回り
	Array<Key> keys = { KeyD, KeyE, KeyW, KeyQ, KeyA, KeyZ, KeyX, KeyC, KeyS };

	for (int i : step(9))
	{
		if (keys[i].down())
		{
			return Optional<Direction>(static_cast<Direction>(i));
		}
	}
	
	return dir;
}

void KeyBoardTeam::Update(const Field & field)
{
	if (_next_steps[0].has_value() && _next_steps[1].has_value())
	{
		return;
	}

	int index = (_next_steps[0].has_value() ? 1 : 0);

	Optional<Direction> next_dir = DecideDirectionByKey();

	if (!next_dir.has_value())
	{
		return;
	}

	// 二人目のエージェントの行動が決まったら準備完了
	if (index == 1)
	{
		_is_ready = true;
	}

	// エージェントの座標から指定の方向に進んだ座標
	Point next_pos = _agents[index].GetPosition().moveBy(Transform::DirToDelta(next_dir.value()));

	if (next_dir.value() == Direction::Stop)
	{
		_next_steps[index] = Step{ Action::Stop, next_dir.value() };
		return;
	}

	// 進んだ先のタイルの有無でアクションを決める
	if (field.GetCells()[next_pos.y][next_pos.x].GetTile() == TileType::None)
	{
		_next_steps[index] = Step{ Action::Move, next_dir.value() };
	}
	else
	{
		_next_steps[index] = Step{ Action::RemoveTile, next_dir.value() };
	}

}

KeyBoardTeam::KeyBoardTeam(TeamType type)
	: Team(type, Agent(), Agent())
{
	_is_ready = false;
}

KeyBoardTeam::KeyBoardTeam(TeamType type, Agent agent1, Agent agent2)
	: Team(type, agent1, agent2)
{
	_is_ready = false;
}

KeyBoardTeam::~KeyBoardTeam()
{
}
