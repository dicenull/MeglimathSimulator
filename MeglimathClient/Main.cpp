
# include <Siv3D.hpp> // OpenSiv3D v0.2.5
#include <HamFramework.hpp>
#include <rapidjson\document.h>
#include <Windows.h> //コマンドライン引数用

#include "../MeglimathCore/TCPString.hpp"
#include "../MeglimathCore/GameInfo.h"
#include "../MeglimathCore/CreateJson.h"

#include "ClientDrawer.h"
#include "KeyboardClient.h"
#include "RandomClient.h"
#include "T_Monte_Carlo.h"
#include "NextBestClient.h"
#include "DoubleNextBestClient.h"
#include "UIClient.h"
#include "RatioClient.h"
#include "DoubleNextBestClient2.h"
#include "BruteForceClient.h"
#include "DoubleBruteForceClient.h"

struct GameData
{
	asc::TCPStringClient tcp_client;
	std::unique_ptr<Client> user_client;
	ClientDrawer drawer;
	GameInfo info;
	TeamType teamType;
	int commander_id;
	int client_id;
};

using MyApp = SceneManager<String, GameData>;

namespace Scenes
{
	class SelectTeamType : public MyApp::Scene
	{
	public:
		SelectTeamType(const InitData& init) : IScene(init)
		{
			Print << U"左 : 赤,右 : 青";
		}

		void update() override
		{
			int commander_id = getData().commander_id;

			//if (KeyLeft.down())
			if (commander_id == 0)
			{
				getData().teamType = TeamType::Red;
				changeScene(U"SetClient", 0);
			}

			//if (KeyRight.down())
			if (commander_id == 1)
			{
				getData().teamType = TeamType::Blue;
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
			clients.push_back(std::make_unique<UIClient>(type));
			clients.push_back(std::make_unique<RatioClient>(type));
			clients.push_back(std::make_unique<DoubleNextBestClient2>(type));
			clients.push_back(std::make_unique<BruteForceClient>(type));
			clients.push_back(std::make_unique<DoubleBruteForceClient>(type));

			for (int i = 0; i < clients.size(); i++)
			{
				if (clients[i] == nullptr)
				{
					continue;
				}

				Print << i << U" : " << clients[i]->Name();
			}
		}

		void update() override
		{
			/*
			for (int i = 0; i < clients.size(); i++)
			{
				Key key = { InputDevice::Keyboard, (uint8)(Key0.code() + i) };

				if (key.down())
				{
					getData().user_client = std::move(clients[i]);
					ClearPrint();
					changeScene(U"Connection", 0);
				}
			}
			*/
			int client_id = getData().client_id;

			getData().user_client = std::move(clients[client_id]);
			ClearPrint();
			changeScene(U"Connection", 0);
		}

		void draw() const override
		{
		}
	};

	class Connection : public MyApp::Scene
	{
	private:
		int count;
	public:
		Connection(const InitData& init) : IScene(init)
		{
			getData().tcp_client.connect(IPv4::localhost(), 31400);
			count = 60;
		}

		void update() override
		{
			if (getData().tcp_client.isConnected())
			{
				changeScene(U"HandShake", 0);
			}

			if (count < 0)
			{
				changeScene(U"Connection", 0.5);
			}
			count--;
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
		{
			// チーム情報を送信
			String send_team = Transform::ToString(getData().teamType);
			send_team += U"\n";
			getData().tcp_client.sendString(send_team);
		}

		void update() override
		{
			auto & tcp_client = getData().tcp_client;

			if (tcp_client.hasError())
			{
				tcp_client.disconnect();

				changeScene(U"Connection", 0);
				return;
			}

			String string_dat;
			tcp_client.readLine(string_dat);

			if (string_dat.isEmpty())
			{
				return;
			}

			auto res = string_dat;
			res.remove(U"\n");

			if (res == U"OK")
			{
				changeScene(U"Game", 0);
				return;
			}

			if (res == U"Type")
			{
				// チームを反転
				getData().teamType = Transform::GetInverseTeam(getData().teamType);
				changeScene(U"HandShake", 0);
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
		bool _first = false;
		
	public:
		Game(const InitData& init) : IScene(init)
		{
			auto type = getData().teamType;

			Window::SetTitle(getData().user_client->Name(), U"Client ", Transform::ToString(type));

			getData().user_client->type = type;
			getData().user_client->Initialize();
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

			if (_is_init)
			{
				// Clientを更新
				user_client->Update(data.info);

				// ServerにThinkを送信
				if (user_client->IsReady())
				{
					auto think = user_client->GetNextThink();
					auto str = Unicode::Widen(Transform::CreateJson(think));
					str.push_back('\n');

					getData().tcp_client.sendString(str);

					user_client->Initialize();
					_first = true;
				}
			}

			// ゲームの更新
			String json_dat;
			getData().tcp_client.readLine(json_dat);

			if (json_dat.isEmpty())
			{
				return;
			}

			rapidjson::Document document;
			document.Parse(json_dat.narrow().data());

			// フィールド情報を更新
			getData().info = { json_dat.narrow() };
			_is_init = true;
		}

		void draw() const override
		{
			if (!_is_init)
			{
				return;
			}


			if (getData().user_client->IsDraw())
			{
				getData().user_client->Draw();
				return;
			}

			if (_first)
			{
				getData().drawer.DrawInstraction(*(getData().user_client));
			}
		}
	};
}

void Main()
{
	int argc;
	LPWSTR *argv = ::CommandLineToArgvW(::GetCommandLineW(), &argc);

	int commander_id = Parse<int>(Unicode::FromWString(argv[1]));
	int client_id = Parse<int>(Unicode::FromWString(argv[2]));

	MyApp manager;
	manager
		.add<Scenes::SelectTeamType>(U"SelectTeamType")
		.add<Scenes::Connection>(U"Connection")
		.add<Scenes::Game>(U"Game")
		.add<Scenes::HandShake>(U"HandShake")
		.add<Scenes::SetClient>(U"SetClient");
	
	const auto p = manager.get();
	p->commander_id = commander_id;
	p->client_id = client_id;

	FontAsset::Register(U"Msg", 64);
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
