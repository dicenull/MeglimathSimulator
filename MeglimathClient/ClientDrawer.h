#pragma once
#include "../MeglimathCore/Drawer.h"
#include "../MeglimathClient/ManualClient.h"

class ClientDrawer
	: public Drawer
{
public:
	void DrawInputState(ManualClient& client);
public:
	ClientDrawer();
	~ClientDrawer();
};

