
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
#include "NextBestClient.h"
#include "DoubleNextBestClient.h"

struct GameData
{
	asc::TCPStringClient tcp_client;
	std::unique_ptr<Client> user_client;
	ClientDrawer drawer;
	GameInfo info;
	TeamType teamType;
};

using MyApp = SceneManager<String, GameData>;

namespace Scenes
{
	class SelectTeamType : public MyApp::Scene
	{
	public:
		SelectTeamType(const InitData& init) : IScene(init)
		{
			Print << U"左 : 赤(B),右 : 青(A)";
		}

		void update() override
		{
			if (KeyLeft.down())
			{
				getData().teamType = TeamType::B;
				changeScene(U"SetClient", 0);
			}

			if (KeyRight.down())
			{
				getData().teamType = TeamType::A;
				changeScene(U"SetClient", 0);
			}
		}

	};

	class SetClient : public MyApp::Scene
	{
	private:
		std::vector<std::unique_ptr<Client>> clients;

	public:
		SetClient(const InitData& init) : IScene(init)
		{
			auto type = getData().teamType;
			clients.push_back(std::make_unique<T_Monte_Carlo>(type));
			clients.push_back(std::make_unique<RandomClient>(type));
			clients.push_back(std::unique_ptr<Client>(new KeyboardClient(type, { KeyD, KeyE, KeyW, KeyQ, KeyA, KeyZ, KeyX, KeyC, KeyS }, KeyShift)));
			clients.push_back(std::make_unique<NextBestClient>(type));
			clients.push_back(std::make_unique<DoubleNextBestClient>(type));
		}

		void update() override
		{
			for (int i = 0; i < clients.size(); i++)
			{
				Key key = { InputDevice::Keyboard, (uint8)(Key0.code() + i) };

				if (key.down())
				{
					getData().user_client = std::move(clients[i]);
					ClearPrint();
					changeScene(U"HandShake", 0);
				}
			}
		}

		void draw() const override
		{
			ClearPrint();
			for (int i = 0; i < clients.size(); i++)
			{
				if (clients[i] == nullptr)
				{
					ClearPrint();
					continue;
				}

				Print << i << U" : " << clients[i]->Name();
			}
		}
	};

	class Connection : public MyApp::Scene
	{
	public:
		Connection(const InitData& init) : IScene(init)
		{
			getData().tcp_client.connect(IPv4::localhost(), 31400);
		}

		void update() override
		{
			if (getData().tcp_client.isConnected())
			{
				changeScene(U"Game", 0);
			}
		}

		void draw() const override
		{
			FontAsset(U"Msg")(U"接続中...").drawAt(Window::Center());
		}
	};

	class HandShake : public MyApp::Scene
	{
	public:
		HandShake(const InitData& init) : IScene(init)
		{ }

		void update() override
		{
			auto & tcp_client = getData().tcp_client;

			if (tcp_client.hasError())
			{
				tcp_client.disconnect();

				changeScene(U"Connection", 0);
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
				changeScene(U"SetClient", 0);
				return;
			}
		}

		void draw() const override
		{
			FontAsset(U"Msg")(U"パラメータ設定中...").drawAt(Window::Center());
		}
	};

	class Game : public MyApp::Scene
	{
	private:
		bool _is_init = false;
		bool _is_update = false;

	public:
		Game(const InitData& init) : IScene(init)
		{
			auto type = getData().teamType;

			Window::SetTitle(getData().user_client->Name(), U"Client ", Transform::ToString(type));
		}

		void update() override
		{
			auto & data = getData();
			auto & user_client = data.user_client;

			if (data.tcp_client.hasError())
			{
				data.tcp_client.disconnect();

				changeScene(U"Connection", 0);
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
			user_client->Update(data.info);

			// ServerにThinkを送信
			if (user_client->IsReady())
			{
				auto think = user_client->GetNextThink();
				auto str = Unicode::Widen(Transform::CreateJson(think));
				str.push_back('\n');

				getData().tcp_client.sendString(str);
				_is_update = false;

				user_client->Initialize();
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

			getData().drawer.DrawInputState(*(getData().user_client));
		}
	};
}

void Main()
{
	MyApp manager;
	manager
		.add<Scenes::SelectTeamType>(U"SelectTeamType")
		.add<Scenes::Connection>(U"Connection")
		.add<Scenes::Game>(U"Game")
		.add<Scenes::HandShake>(U"HandShake")
		.add<Scenes::SetClient>(U"SetClient");

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
