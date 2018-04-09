
# include <Siv3D.hpp> // OpenSiv3D v0.2.4
#include <HamFramework.hpp>
#include "Game.h"

struct CommonData
{
	Game game =
	{
		Field(Size(10, 10)),
		Team(TeamType::A, Agent(Point(1,1)), Agent(Point(5,6))),
		Team(TeamType::B, Agent(Point(5, 1)), Agent(Point(3, 7))) };
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

		}

		void update() override
		{
			getData().game.Update();
		}

		void draw() const override
		{
			getData().game.Draw();
		}
	};
}

void Main()
{
	MyApp manager;
	manager.add<Scenes::Game>(U"Game");
	
	FontAsset::Register(U"Cell", 16, Typeface::Black);

	while (System::Update())
	{
		if (!manager.update())
		{
			break;
		}
	}
}
