#pragma once
#include "ManualClient.h"
class KeyboardClient :
	public ManualClient
{
private:
	Array<Key> _operation_keys;

protected:
	Optional<Direction> decideDirection() override
	{
		Optional<Direction> dir = none;

		// Rightから反時計回りに方向キーを指定
		for (int i : step(9))
		{
			if (_operation_keys[i].down())
			{
				_is_pressed_shift = KeyLShift.pressed();

				return Optional<Direction>(static_cast<Direction>(i));
			}
		}

		return dir;
	}

public:
	KeyboardClient(TeamType type, Array<Key> operation_keys);
	~KeyboardClient();
};

