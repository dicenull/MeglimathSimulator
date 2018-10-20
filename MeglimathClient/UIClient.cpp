#include "UIClient.h"



UIClient::UIClient(TeamType type)
{
	_type = type;

	coms[0].setCenter({ 50, Window::Size().y / 2 });
	coms[1].setCenter({ Window::Size().x - 50, Window::Size().y / 2 });
}
