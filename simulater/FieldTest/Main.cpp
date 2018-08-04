
# include <Siv3D.hpp> // OpenSiv3D v0.2.5
#include "../MeglimathCore/GameLogic/Field.h"
#include "../MeglimathCore/Drawer.h"

void Main()
{
	FontAsset::Register(U"Cell", 16, Typeface::Black);
	FontAsset::Register(U"Test", 16, Typeface::Default);
	
	Drawer drawer;
	Array<String> paths = FileSystem::DirectoryContents(U"../TestFields");
	int index = 0;

	Field field;
	JSONReader json;
	
	const Texture failure(Emoji(U"❌"), TextureDesc::Mipped);
	const Texture success(Emoji(U"✔"), TextureDesc::Mipped);

	auto judge = [&](std::pair<int, int> points) {return (points.first == points.second) ? success : failure; };

	std::pair<int, int> points_a;
	std::pair<int, int> points_b;
	while (System::Update())
	{
		if (index < paths.size() && MouseL.down())
		{
			TextReader reader(paths[index]);
			field = Field{ reader.readAll().narrow() };
			json = JSONReader(paths[index]);

			index++;

			field.UpdatePoint();
			points_a.first = json[U"TeamA"].get<int>();
			points_a.second = field.GetAreaPoints()[0];

			points_b.first = json[U"TeamB"].get<int>();
			points_b.second = field.GetAreaPoints()[1];
		}

		drawer.DrawField(field);

		judge(points_a).resized(16).draw(Point(0, 5));
		FontAsset(U"Test")(U"TileA  expected : ", points_a.first, U" actual : ", points_a.second).draw(Point(16, 0));

		judge(points_b).resized(16).draw(Point(0, 20));
		FontAsset(U"Test")(U"TileB  expected : ", points_b.first, U" actual : ", points_b.second).draw(Point(16, 16));
	}
}
