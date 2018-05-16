#include "ManualClient.h"



bool ManualClient::IsReady()
{
	return _is_ready;
}

void ManualClient::SetTeamType(TeamType type)
{
	_type = type;
}

ManualClient::ManualClient()
{
}


ManualClient::ManualClient(TeamType type)
	: Client(type)
{
}

ManualClient::~ManualClient()
{
}
