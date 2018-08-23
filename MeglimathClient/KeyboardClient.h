#pragma once
#include "ManualClient.h"
class KeyboardClient :
	public ManualClient
{
private:
	Array<Key> _operation_keys;
	Key _toggle_key;

public:
	String Name() override
	{
		return U"Keyboard";
	}

protected:
	Optional<Step> decideStep() override
	{
		// Rightから反時計回りに方向キーを指定
		for (int i : step(9))
		{
			if (_operation_keys[i].down())
			{
				Direction dir = static_cast<Direction>(i);

				Action action;
				if (_toggle_key.pressed())
				{
					action = Action::RemoveTile;
				}
				else
				{
					action = Action::Move;
				}

				return Optional<Step>({ action, dir });
			}
		}

		return none;
	}

public:
	KeyboardClient(TeamType type, Array<Key> operation_keys, Key toggle_key);
	~KeyboardClient();
};

