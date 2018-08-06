#include "ClientDrawer.h"
#include "Transform.h"

void ClientDrawer::DrawInputState(ManualClient& client)
{
	auto steps = client.GetSteps();
	for (auto client_step : steps)
	{
		if (!client_step.has_value())
		{
			continue;
		}

		FontAsset(U"Msg")(
			Transform::ToString(client_step.value().action))
			.draw(statOrigin);
		FontAsset(U"Msg")(
			Transform::ToString(client_step.value().direction))
			.draw(statOrigin + Point(0, 32));

	}
}

ClientDrawer::ClientDrawer()
{
}


ClientDrawer::~ClientDrawer()
{
}
