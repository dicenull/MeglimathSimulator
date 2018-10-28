#include "KeyboardClient.h"


KeyboardClient::KeyboardClient(TeamType type, Array<Key> operation_keys, Key toggle_key)
{
	type = type;
	_operation_keys = operation_keys;
	_toggle_key = toggle_key;
}

KeyboardClient::~KeyboardClient()
{
}
