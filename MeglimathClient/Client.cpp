#include "Client.h"


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
