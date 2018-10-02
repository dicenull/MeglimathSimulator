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

ClientDrawer::ClientDrawer()
{
}


ClientDrawer::~ClientDrawer()
{
}
