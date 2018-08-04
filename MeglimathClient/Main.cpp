
# include <Siv3D.hpp> // OpenSiv3D v0.2.5
#include <HamFramework.hpp>
#include <rapidjson\document.h>

#include "../MeglimathCore/TCPString.hpp"
#include "../MeglimathCore/GameInfo.h"
#include "../MeglimathCore/Drawer.h"
#include "../MeglimathCore/CreateJson.h"

#include "KeyboardClient.h"
#include "RandomClient.h"

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
		std::unique_ptr<Client> user_client;

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

			if (!json_dat.isEmpty())
			{
				rapidjson::Document document;
				document.Parse(json_dat.narrow().data());

				if (document.HasMember("TeamType"))
				{
					auto team_type_ch = document["TeamType"].GetString()[0];
					Optional<TeamType> type;

					if (team_type_ch == 'A')
					{
						type = TeamType::A;
					}
					else if (team_type_ch == 'B')
					{
						type = TeamType::B;
					}
					
					if (type.has_value())
					{
						// Clientを初期化
						user_client.reset(new KeyboardClient(type.value(), { KeyD, KeyE, KeyW, KeyQ, KeyA, KeyZ, KeyX, KeyC, KeyS }));
						// user_client.reset(new RandomClient(type.value()));
					}
				}
				else if (user_client)
				{
					getData().info = { json_dat.narrow() };
					_is_init = true;
				}
				else
				{
					getData().tcp_client.sendString(U"0\n");
				}
			}

			if (_is_init)
			{
				auto & client = user_client;
				
				// Clientを更新
				client->Update();

				// ServerにThinkを送信
				if (client->IsReady())
				{
					auto think = client->NextThink(data.info);
					auto str = Unicode::Widen(Transform::CreateJson(think));
					str.push_back('\n');

					getData().tcp_client.sendString(str);
				}
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
