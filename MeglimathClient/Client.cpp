#include "Client.h"


Think Client::GetNextThink()
{
	return _think;
}

void Client::Initialize()
{
	_is_ready = false;
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
	type = type;
}
