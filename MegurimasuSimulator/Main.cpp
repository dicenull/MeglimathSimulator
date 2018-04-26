
# include <Siv3D.hpp> // OpenSiv3D v0.2.4
#include <HamFramework.hpp>
#include "Game.h"
#include "RandomTeam.h"
#include "KeyBoardTeam.h"
#include "GamePadTeam.h"

struct CommonData
{
	const String field_path = U"../Fields/LargeField.json";
	Game game =
	{
		std::shared_ptr<Team>(new GamePadTeam(TeamType::A)),
		std::shared_ptr<Team>(new KeyBoardTeam(TeamType::B,
			{ KeyD, KeyE, KeyW, KeyQ, KeyA, KeyZ, KeyX, KeyC, KeyS }))
	};
	Drawer drawer;
};

using MyApp = SceneManager<String, CommonData>;

namespace Scenes
{
	struct Game : MyApp::Scene
	{
	public:
		Game(const InitData& init)
			: IScene(init)
		{
			auto & data = getData();

			data.game.InitalizeFromJson(data.field_path);
		}

		void update() override
		{
			getData().game.Update();

			if (getData().game.IsReady())
			{
				getData().game.NextTurn();
			}
		}

		void draw() const override
		{
			auto & game = getData().game;
			auto & drawer = getData().drawer;

			drawer.DrawField(game.GetField(), game.GetAgents());
			drawer.DrawAgents(game.GetAgentMap());
			drawer.DrawStatus(game.GetThinks(), game.GetField(), game.GetTurn());
		}
	};
}

void Main()
{
	MyApp manager;
	manager.add<Scenes::Game>(U"Game");
	
	FontAsset::Register(U"Cell", 16, Typeface::Black);
	FontAsset::Register(U"Stat", 16, Typeface::Default);

	while (System::Update())
	{
		if (!manager.update())
		{
			break;
		}
	}
}
