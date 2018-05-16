
# include <Siv3D.hpp> // OpenSiv3D v0.2.5
#include "../MeglimathCore/TCPString.hpp"
#include "../MeglimathCore/GameInfo.h"
#include "../MeglimathCore/Drawer.h"
#include "../MeglimathCore/CreateJson.h"
#include <HamFramework.hpp>

struct GameData
{
	asc::TCPStringClient tcp_client;
	Drawer drawer;
	GameInfo info;
};

using MyApp = SceneManager<String, GameData>;

namespace Scenes
{
	struct Connection : MyApp::Scene
	{
		Connection(const InitData& init) : IScene(init)
		{
			getData().tcp_client.connect(IPv4::localhost(), 31400);
		}

		void update() override
		{
			if (getData().tcp_client.isConnected())
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
		bool _is_init = false;

		Game(const InitData& init) : IScene(init)
		{
		}

		void update() override
		{
			auto & data = getData();
			if (data.tcp_client.hasError())
			{
				data.tcp_client.disconnect();

				changeScene(U"Connection");
			}

			// ランダムな行動を動作確認として送る
			Think test_thinks =
			{
				Step{ Action(Random(0,1)),Direction(Random(0,7)) },
				Step{ Action(Random(0,1)),Direction(Random(0,7)) }
			};
			
			auto str = Unicode::Widen(Transform::CreateJson(test_thinks));
			str.push_back('\n');

			getData().tcp_client.sendString(str);
			// ---

			String json_dat;
			getData().tcp_client.readLine(json_dat);

			if (json_dat.isEmpty())
			{
				return;
			}
			
			getData().info = { json_dat.narrow() };
			_is_init = true;
		}

		void draw() const override
		{
			if (!_is_init)
			{
				FontAsset(U"Msg")(U"待機中...").drawAt(Window::Center());
				return;
			}

			getData().drawer.DrawField(getData().info.GetField());
			getData().drawer.DrawAgents(getData().info.GetAllAgent());
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
	FontAsset::Register(U"Cell", 16, Typeface::Black);

	Window::SetTitle(U"TCP Client");

	while (System::Update())
	{
		if (!manager.update())
		{
			break;
		}
	}
}
