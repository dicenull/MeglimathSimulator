#include "ManualClient.h"



void ManualClient::SetTeamType(TeamType type)
{
	type = type;
}

Array<Optional<Step>> ManualClient::GetSteps()
{
	Array<Optional<Step>> steps;
	steps.append({ _steps[0], _steps[1] });
	return steps;
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
