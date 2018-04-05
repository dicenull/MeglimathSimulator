#pragma once
#include <Siv3D.hpp>

enum class TileType
{
	A, B, None
};

enum class TeamType
{
	A, B
};

namespace Tile
{
	/// <summary>
	/// ƒ^ƒCƒ‹‚ÌF‚ğæ“¾‚·‚é
	/// </summary>
	Color ColorOf(TileType t_type);
}