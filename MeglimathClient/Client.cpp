#include "Client.h"


Think Client::GetNextThink()
{
	return _think;
}

void Client::Draw()
{
}

bool Client::IsReady()
{
	return _is_ready;
}

bool Client::IsDraw()
{
	return false;
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
