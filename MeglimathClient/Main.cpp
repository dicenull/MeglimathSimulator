
# include <Siv3D.hpp> // OpenSiv3D v0.2.5
#include "../MeglimathCore/TCPString.hpp"
#include <HamFramework.hpp>

struct GameData
{
	asc::TCPStringClient client;
};

using MyApp = SceneManager<String, GameData>;

namespace Scenes
{
	struct Connection : MyApp::Scene
	{
		Connection(const InitData& init) : IScene(init)
		{
			getData().client.connect(IPv4::localhost(), 31400);
		}

		void update() override
		{
			if (getData().client.isConnected())
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
			String json_dat;
			getData().client.readLine(json_dat);

			Print << json_dat;
		}

		void update() override
		{
			auto & data = getData();
			if (data.client.hasError())
			{
				data.client.disconnect();

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

	Window::SetTitle(U"TCP Client");

	while (System::Update())
	{
		if (!manager.update())
		{
			break;
		}
	}
}
