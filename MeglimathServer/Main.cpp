
# include <Siv3D.hpp> // OpenSiv3D v0.2.5
#include <HamFramework.hpp>

#include <rapidjson\document.h>

#include "../MeglimathCore/Game.h"
#include "../MeglimathCore/Drawer.h"
#include "../MeglimathCore/TCPString.hpp"
#include "../MeglimathCore/CreateJson.h"

struct GameData
{
	const FilePath field_path = U"../Fields/LargeField.json";
	Game game = { field_path };
	Drawer drawer;
	Optional<Think> thinks[2] = { none, none };
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

	public:
		Game(const InitData& init) : IScene(init)
		{
			sendGameInfo();
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

			String json_dat;
			getData().server.readLine(json_dat);

			if (json_dat.isEmpty())
			{
				return;
			}

			auto & thinks = getData().thinks;

			rapidjson::Document doc;
			doc.Parse(json_dat.narrow().data());
			Think think = { json_dat.narrow() };
			auto team_type = doc["TeamType"].GetString();

			if (team_type[0] == 'A')
			{
				thinks[0] = think;
			}

			if (team_type[0] == 'B')
			{
				thinks[1] = think;
			}

			if (thinks[0].has_value() && thinks[1].has_value())
			{
				getData().game.NextTurn(thinks[0].value(), thinks[1].value());
				thinks[0] = none;
				thinks[1] = none;

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
