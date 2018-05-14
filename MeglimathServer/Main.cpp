
# include <Siv3D.hpp> // OpenSiv3D v0.2.5
#include <HamFramework.hpp>

#include <rapidjson\document.h>

#include "../MeglimathCore/Game.h"
#include "../MeglimathCore/Drawer.h"
#include "../MeglimathCore/TCPString.hpp"

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
			FontAsset(U"Msg")(U"接続中...\n", getData().server.num_sessions()).drawAt(Window::Center());
		}
	};

	struct Game : MyApp::Scene
	{
		Game(const InitData& init) : IScene(init)
		{
			auto str = Unicode::Widen(getData().game.GetGameInfo().CreateJson());
			str.push_back('\n');

			for (auto id : getData().server.getSessionIDs())
			{
				getData().server.sendString(str, id);
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

			if (team_type == "A")
			{
				thinks[0] = think;
			}

			if (team_type == "B")
			{
				thinks[1] = think;
			}

			if (thinks[0].has_value() && thinks[1].has_value())
			{
				getData().game.NextTurn(thinks[0].value(), thinks[1].value());
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

	Window::SetTitle(U"TCP Server");

	while (System::Update())
	{
		if (!manager.update())
		{
			break;
		}
	}
}
