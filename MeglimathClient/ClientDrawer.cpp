#include "ClientDrawer.h"
#include "../MeglimathCore/GameLogic/Transform.h"

void ClientDrawer::DrawInputState(Client& client)
{
	auto steps = client.GetNextThink().steps;
	auto drawPos = statOrigin;

	for (int i = 0; i < 2; i++)
	{
		FontAsset(U"Msg")(
			Transform::ToString(steps[i].action))
			.draw(drawPos);

		drawPos += Point(0, 32);
		FontAsset(U"Msg")(
			Transform::ToString(steps[i].direction))
			.draw(drawPos);

		drawPos += Point(0, 32);
	}
}

void ClientDrawer::DrawInstraction(Client & client)
{
	auto steps = client.GetNextThink().steps;
	auto drawPos = Point(0, 0);

	for (auto step : steps)
	{
		String instraction = U"";
		if (step.action == Action::RemoveTile)
		{
			instraction += U"横";
		}

		instraction += Format(Transform::ToNumPad(client.type, step.direction));
		
		FontAsset(U"Msg")(instraction).draw(drawPos);
		drawPos += Point(Window::Size().x / 2, 0);
	}
}

ClientDrawer::ClientDrawer()
{
}


ClientDrawer::~ClientDrawer()
{
}
