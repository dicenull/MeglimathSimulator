#include "ManualClient.h"



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
