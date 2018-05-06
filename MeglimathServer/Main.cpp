
# include <Siv3D.hpp> // OpenSiv3D v0.2.5
#include <HamFramework.hpp>
#include "../MeglimathCore/Game.h"

struct GameData
{
	TCPServer server;
};

using MyApp = SceneManager<String, GameData>;

namespace Scenes
{
	struct Connection : MyApp::Scene
	{
		Connection(const InitData& init) : IScene(init)
		{
			getData().server.startAccept(31400);
		}

		void update() override
		{
			if (getData().server.hasSession())
			{
				changeScene(U"Game");
			}
		}

		void draw() const override
		{
			FontAsset(U"Msg")(U"接続中...").drawAt(Window::Center());
		}
	};

	struct Game : MyApp::Scene
	{
		Game(const InitData& init) : IScene(init)
		{

		}

		void update() override
		{
			auto & data = getData();
			if (!data.server.hasSession())
			{
				data.server.disconnect();

				changeScene(U"Connection");
			}
		}

		void draw() const override
		{

		}
	};
}

void Main()
{
	MyApp manager;
	manager
		.add<Scenes::Connection>(U"Connection")
		.add<Scenes::Game>(U"Game");

	FontAsset::Register(U"Msg", 32);

	Window::SetTitle(U"TCP Server");

	while (System::Update())
	{
		if (!manager.update())
		{
			break;
		}
	}
}
