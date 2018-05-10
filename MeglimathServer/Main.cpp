
# include <Siv3D.hpp> // OpenSiv3D v0.2.5
#include <HamFramework.hpp>

#include <cereal\cereal.hpp>
#include <cereal\archives\json.hpp>

#include "../MeglimathCore/Game.h"
#include "../MeglimathCore/TCPString.hpp"

struct GameData
{
	const FilePath field_path = U"../Fields/LargeField.json";
	Game game = { field_path };
	asc::TCPStringServer server;
};

using MyApp = SceneManager<String, GameData>;

namespace Scenes
{
	struct Connection : MyApp::Scene
	{
		Connection(const InitData& init) : IScene(init)
		{
			getData().server.startAccept(31400);
		}

		void update() override
		{
			if (getData().server.hasSession())
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
		Game(const InitData& init) : IScene(init)
		{
			std::stringstream ss;

			{
				cereal::JSONOutputArchive o_archive(ss);

				o_archive(cereal::make_nvp("info", getData().game.GetGameInfo()));
			}

			getData().server.sendString(Format(ss.str()));
		}

		void update() override
		{
			auto & data = getData();
			if (!data.server.hasSession())
			{
				data.server.disconnect();

				changeScene(U"Connection");
			}
		}

		void draw() const override
		{

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
