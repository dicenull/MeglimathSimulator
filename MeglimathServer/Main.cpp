
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
	const FilePath field_path = U"../Fields/LargeField.json";
	Game game = { field_path };
	Drawer drawer;
	std::map<SessionID, Optional<Think>> thinks;
	asc::TCPStringServer server;
};

using MyApp = SceneManager<String, GameData>;

namespace Scenes
{
	struct Connection : MyApp::Scene
	{
		Connection(const InitData& init) : IScene(init)
		{
			getData().server.startAcceptMulti(31400);
		}

		void update() override
		{
			if (getData().server.num_sessions() == 2)
			{
				changeScene(U"Game");
			}
		}

		void draw() const override
		{
			getData().drawer.DrawField(getData().game.GetField());
			FontAsset(U"Msg")(U"待機中... 接続数 : ", getData().server.num_sessions()).draw(Point(0, 0));
		}
	};

	struct Game : MyApp::Scene
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

		void sendTeamTypes()
		{
			auto ids = getData().server.getSessionIDs();
			auto to_wide_json = [](TeamType type) {return Unicode::Widen(Transform::CreateJson(type)) + U"\n"; };

			getData().server.sendString(to_wide_json(TeamType::A), ids[0]);
			getData().server.sendString(to_wide_json(TeamType::B), ids[1]);
		}

	public:
		Game(const InitData& init) : IScene(init)
		{
			sendTeamTypes();
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

				changeScene(U"Connection");
			}

			// Clientから次ターンの行動を受け取る
			for (auto id : data.server.getSessionIDs())
			{
				String json_dat;
				getData().server.readUntil('\n', json_dat, Optional<SessionID>(id));

				if (json_dat.isEmpty())
				{
					continue;
				}

				if (json_dat == U"0")
				{
					sendTeamTypes();
					continue;
				}

				auto & thinks = getData().thinks;

				// jsonからThink情報を取得
				rapidjson::Document doc;
				doc.Parse(json_dat.narrow().data());

				data.thinks[id] = { json_dat.narrow() };
			}
			
			// Client二つ分のThinkが更新されたら次のターンへ
			auto ids = data.server.getSessionIDs();
			auto &thinks = data.thinks;
			if (thinks[ids[0]].has_value() && thinks[ids[1]].has_value())
			{
				getData().game.NextTurn(thinks[ids[0]].value(), thinks[ids[1]].value());
				thinks[ids[0]] = none;
				thinks[ids[1]] = none;

				sendGameInfo();
			}
		}

		void draw() const override
		{
			auto & game = getData().game;
			auto & drawer = getData().drawer;

			drawer.DrawField(game.GetField());
			drawer.DrawAgents(game.GetAgentMap());
			drawer.DrawStatus(game.GetThinks(), game.GetField(), game.GetTurn());
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
