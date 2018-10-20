#include "Client.h"


Think Client::GetNextThink()
{
	return _think;
}

bool Client::IsReady()
{
	return _is_ready;
}

Client::Client()
{
}


Client::Client(TeamType type)
{
	_type = type;
}

Client::~Client()
{
}
