#pragma once
#include <Siv3D.hpp>
#include <cereal/cereal.hpp>
#include <cereal/archives/json.hpp>
#include "Cell.h"

namespace s3d
{
	template<class Archive>
	void serialize(Archive & archive, Size & size)
	{
		archive(cereal::make_nvp("Size", ToString(size));
	}

}

template<class Archive>
void serialize(Archive & archive, Grid<Cell> & cells)
{
	
	std::vector<int> cell_data;
	for (auto y : step((cells.size().y + 1) / 2))
	{
		for (auto x : step((cells.size().x + 1) / 2))
		{
			cell_data.push_back(cells[y][x].GetPoint());
		}
	}

	archive(cereal::make_nvp("Cells", cell_data));

	std::vector<std::string> tiles;
	for (auto y : step(cells.size().y))
	{
		std::string str;
		for (auto x : step(cells.size().x))
		{
			str.push_back(ToString(cells[y][x].GetTile()));
		}

		tiles.push_back(str);
	}

	archive(cereal::make_nvp("Tiles", tiles));
}