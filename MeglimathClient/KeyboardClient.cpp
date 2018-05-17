#include "KeyboardClient.h"


KeyboardClient::KeyboardClient(TeamType type, Array<Key> operation_keys)
{
	_type = type;
	_operation_keys = operation_keys;
}

KeyboardClient::~KeyboardClient()
{
}
