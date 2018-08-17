
# include <Siv3D.hpp> // OpenSiv3D v0.2.5
#include <HamFramework.hpp>
#include <rapidjson\document.h>

#include "../MeglimathCore/TCPString.hpp"
#include "../MeglimathCore/GameInfo.h"
#include "../MeglimathCore/CreateJson.h"

#include "ClientDrawer.h"
#include "KeyboardClient.h"
#include "RandomClient.h"
#include "T_Monte_Carlo.h"

struct GameData
{
	asc::TCPStringClient tcp_client;
	ClientDrawer drawer;
	GameInfo info;
	TeamType teamType;
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
				changeScene(U"HandShake");
			}
		}

		void draw() const override
		{
			FontAsset(U"Msg")(U"接続中...").drawAt(Window::Center());
		}
	};

	struct HandShake : MyApp::Scene
	{
		HandShake(const InitData& init) : IScene(init)
		{

		}

		void update() override
		{
			auto & tcp_client = getData().tcp_client;

			if (tcp_client.hasError())
			{
				tcp_client.disconnect();

				changeScene(U"Connection");
				return;
			}

			String json_dat;
			tcp_client.readLine(json_dat);

			if (json_dat.isEmpty())
			{
				tcp_client.sendString(U"Type\n");
				return;
			}

			rapidjson::Document document;
			document.Parse(json_dat.narrow().data());

			if (!document.HasMember("TeamType"))
			{
				return;
			}

			std::string type = document["TeamType"].GetString();
			Optional<TeamType> team_type = none;

			if (type == "A")
			{
				team_type = TeamType::A;
			}

			if (type == "B")
			{
				team_type = TeamType::B;
			}

			if (team_type.has_value())
			{
				getData().teamType = team_type.value();
				tcp_client.sendString(U"OK\n");
				changeScene(U"Game");
				return;
			}
		}

		void draw() const override
		{
			FontAsset(U"Msg")(U"パラメータ設定中...").drawAt(Window::Center());
		}
	};

	struct Game : MyApp::Scene
	{
		std::unique_ptr<Client> user_client;
		bool _is_init = false;
		bool _is_update = false;

		Game(const InitData& init) : IScene(init)
		{
			auto type = getData().teamType;

			// Clientを初期化
			//user_client.reset(new T_Monte_Carlo(type));
			user_client.reset(new KeyboardClient(type, { KeyD, KeyE, KeyW, KeyQ, KeyA, KeyZ, KeyX, KeyC, KeyS }, KeyShift));
			// user_client.reset(new RandomClient(type));

			Window::SetTitle(U"Client ", Transform::ToString(type));
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
			if (!_is_update)
			{
				String json_dat;
				getData().tcp_client.readLine(json_dat);

				if (json_dat.isEmpty())
				{
					return;
				}

				rapidjson::Document document;
				document.Parse(json_dat.narrow().data());

				if (document.HasMember("TeamType"))
				{
					return;
				}

				// フィールド情報を更新
				getData().info = { json_dat.narrow() };
				_is_init = true;

				_is_update = true;
			}

			// Clientを更新
			user_client->Update();

			// ServerにThinkを送信
			if (user_client->IsReady())
			{
				auto think = user_client->NextThink(data.info);
				auto str = Unicode::Widen(Transform::CreateJson(think));
				str.push_back('\n');

				getData().tcp_client.sendString(str);
				_is_update = false;
			}
		}

		void draw() const override
		{
			if (!_is_init)
			{
				return;
			}

			getData().drawer.DrawField(getData().info.GetField());
			getData().drawer.DrawAgents(getData().info.GetAllAgent());

			try
			{
				// 手動クライアントの場合のみ入力状態を描画
				auto& client = dynamic_cast<ManualClient&>(*user_client);
				getData().drawer.DrawInputState(client);
			}
			catch (const std::bad_cast&)
			{
				return;
			}
		}
	};
}

void Main()
{
	MyApp manager;
	manager
		.add<Scenes::Connection>(U"Connection")
		.add<Scenes::Game>(U"Game")
		.add<Scenes::HandShake>(U"HandShake");

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
