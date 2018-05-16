
# include <Siv3D.hpp> // OpenSiv3D v0.2.5
#include <HamFramework.hpp>

#include "../MeglimathCore/TCPString.hpp"
#include "../MeglimathCore/GameInfo.h"
#include "../MeglimathCore/Drawer.h"
#include "../MeglimathCore/CreateJson.h"

#include "KeyboardClient.h"

struct GameData
{
	asc::TCPStringClient tcp_client;
	Drawer drawer;
	GameInfo info;

	KeyboardClient user_client = KeyboardClient({ KeyD, KeyE, KeyW, KeyQ, KeyA, KeyZ, KeyX, KeyC, KeyS });
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

			// ゲームの更新
			String json_dat;
			getData().tcp_client.readLine(json_dat);

			if (json_dat == U"A")
			{
				getData().user_client.SetTeamType(TeamType::A);
			}

			if (json_dat == U"B")
			{
				getData().user_client.SetTeamType(TeamType::B);
			}

			if (!json_dat.isEmpty())
			{
				getData().info = { json_dat.narrow() };
				_is_init = true;
			}

			// Clientを更新
			data.user_client.Update();

			// ServerにThinkを送信
			if (data.user_client.IsReady())
			{
				auto think = data.user_client.NextThink(data.info);
				auto str = Unicode::Widen(Transform::CreateJson(think));
				str.push_back('\n');

				getData().tcp_client.sendString(str);
			}
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
