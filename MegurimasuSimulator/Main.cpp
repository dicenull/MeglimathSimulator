
# include <Siv3D.hpp> // OpenSiv3D v0.2.4
#include <HamFramework.hpp>
#include "Game.h"

struct CommonData
{
	Game game = { Field(Size(10, 10)), Team(), Team() };
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
