
# include <Siv3D.hpp> // OpenSiv3D v0.2.5
#include <HamFramework.hpp>

#include <rapidjson\document.h>
#include <rapidjson\writer.h>

#include "../MeglimathCore/Game.h"
#include "../MeglimathCore/Drawer.h"
#include "../MeglimathCore/TCPString.hpp"
#include "../MeglimathCore/CreateJson.h"

struct GameData
{
	Game game;
	Drawer drawer;
	std::map<SessionID, Optional<Think>> thinks;
	asc::TCPStringServer server;
};

using MyApp = SceneManager<String, GameData>;

namespace Scenes
{
	class ReadFieldJson : public MyApp::Scene
	{
	public:
		ReadFieldJson(const InitData& init) : IScene(init)
		{
			// フィールド情報を受け取るための通信
			getData().server.startAccept(31400);
		}

		void update() override
		{
			auto & server = getData().server;

			if (server.hasSession())
			{
				// フィールド情報を受信
				String field_json;
				server.readLine(field_json);

				if (field_json != U"")
				{
					// ゲームの初期化
					getData().game = { field_json };

					// クライアントとの接続へ移行
					server.disconnect();
					changeScene(U"Connection", 0);
					return;
				}
			}
		}

		void draw() const override
		{
			FontAsset(U"Msg")(U"フィールド情報受信中...").draw(Point::Zero());
		}
	};


	class Connection : public MyApp::Scene
	{
	public:
		Connection(const InitData& init) : IScene(init)
		{
			// 二つのクライアントと接続する
			getData().server.startAcceptMulti(31400);
		}

		void update() override
		{
			if (getData().server.num_sessions() == 2)
			{
				changeScene(U"HandShake", 0);
			}
		}

		void draw() const override
		{
			getData().drawer.DrawField(getData().game.GetField());
			FontAsset(U"Msg")(U"待機中... 接続数 : ", getData().server.num_sessions()).draw(Point(0, 0));
		}
	};

	class HandShake : public MyApp::Scene
	{
	private:
		bool _has_connection[2] = { false, false };

	private:
		void sendTeamTypes()
		{
			auto ids = getData().server.getSessionIDs();
			
			sendTeamType(ids[0], TeamType::A);
			sendTeamType(ids[1], TeamType::B);
		}

		void sendTeamType(size_t id, TeamType type)
		{
			auto to_wide_json = [](TeamType type) {return Unicode::Widen(Transform::CreateJson(type)) + U"\n"; };

			getData().server.sendString(to_wide_json(type), id);
		}

	public:
		HandShake(const InitData& init) : IScene(init)
		{
			sendTeamTypes();
		}

		void update() override
		{
			auto & server = getData().server;

			if (server.num_sessions() != 2)
			{
				server.disconnect();
				server.cancelAccept();

				changeScene(U"Connection", 0);
			}

			for (auto i = 0; i < server.num_sessions(); i++)
			{
				auto ids = server.getSessionIDs();

				String json_dat;
				server.readUntil(U'\n', json_dat, ids[i]);
				json_dat.remove(U'\n');

				if (json_dat.isEmpty())
				{
					continue;
				}

				if (json_dat == U"Type")
				{
					// もう片方が設定完了している場合
					TeamType type;
					if (_has_connection[1 - i])
					{
						type = TeamType::B;
					}
					else
					{
						type = TeamType::A;
					}

					sendTeamType(ids[i], type);
					continue;
				}

				if (json_dat == U"OK")
				{
					_has_connection[i] = true;
				}
			}

			if (_has_connection[0] && _has_connection[1])
			{
				changeScene(U"Game", 0);
				return;
			}
		}

		void draw() const override
		{
			FontAsset(U"Msg")(U"パラメータ設定中...").draw(Point(0,0));
		}
	};

	class Game : public MyApp::Scene
	{
	private:
		void sendGameInfo()
		{
			auto str = Unicode::Widen(Transform::CreateJson(getData().game.GetGameInfo()));
			str.push_back('\n');

			for (auto id : getData().server.getSessionIDs())
			{
				getData().server.sendString(str, id);
			}
		}

	public:
		Game(const InitData& init) : IScene(init)
		{
			sendGameInfo();

			// ClientのThinkをidで管理
			for (auto id : getData().server.getSessionIDs())
			{
				getData().thinks[id] = none;
			}
		}

		void update() override
		{
			auto & data = getData();
			if (data.server.num_sessions() != 2)
			{
				data.server.disconnect();
				data.server.cancelAccept();

				changeScene(U"Connection", 0);
			}

			// Clientから次ターンの行動を受け取る
			for (auto id : data.server.getSessionIDs())
			{
				String json_dat;
				data.server.readUntil('\n', json_dat, Optional<SessionID>(id));

				if (json_dat.isEmpty())
				{
					continue;
				}

				// jsonからThink情報を取得
				rapidjson::Document doc;
				doc.Parse(json_dat.narrow().data());

				data.thinks[id] = Think::makeThink( json_dat.narrow() );
			}
			
			// Client二つ分のThinkが更新されたら次のターンへ
			auto ids = data.server.getSessionIDs();
			
			if (ids.size() != 2)
			{
				return;
			}

			auto &thinks = data.thinks;
			if (thinks[ids[0]].has_value() && thinks[ids[1]].has_value())
			{
				getData().game.NextTurn(thinks[ids[0]].value(), thinks[ids[1]].value());

				// Think情報を初期化
				thinks[ids[0]] = none;
				thinks[ids[1]] = none;

				sendGameInfo();
			}
		}

		void draw() const override
		{
			auto & game = getData().game;
			auto & drawer = getData().drawer;
			auto field = game.GetField();

			drawer.DrawField(field);
			drawer.DrawAgents(game.GetAgentMap());
			drawer.DrawStatus(game.GetThinks(), field, game.GetTurn());
		}
	};
}

void Main()
{
	MyApp manager;
	manager
		.add<Scenes::ReadFieldJson>(U"ReadFieldJson")
		.add<Scenes::Connection>(U"Connection")
		.add<Scenes::Game>(U"Game")
		.add<Scenes::HandShake>(U"HandShake");

	FontAsset::Register(U"Msg", 32);
	FontAsset::Register(U"Cell", 16, Typeface::Black);
	FontAsset::Register(U"Stat", 16, Typeface::Default);

	Window::SetTitle(U"TCP Server");

	while (System::Update())
	{
		if (!manager.update())
		{
			break;
		}
	}
}
