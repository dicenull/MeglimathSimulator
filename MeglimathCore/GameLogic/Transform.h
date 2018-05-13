#pragma once
#include "Types.h"
#include "Action.h"


namespace Transform
{
	
	/// <summary>
	/// •ûŒü‚©‚çˆÚ“®•ûŒü‚É•ÏŠ·‚·‚é
	/// </summary>
	/// <param name="dir">•ÏŠ·Œ³‚Ì•ûŒü</param>
	/// <returns>•ûŒü‚ð•ÏŠ·‚µ‚½ˆÚ“®•ûŒü</returns>
	constexpr _Point<int> DirToDelta(Direction dir)
	{
		switch (dir)
		{
		case Direction::Right:
			return _Point(1, 0);
			
		case Direction::RightUp:
			return _Point(1, -1);
			
		case Direction::Up:
			return _Point(0, -1);
			
		case Direction::LeftUp:
			return _Point(-1, -1);
			
		case Direction::Left:
			return _Point(-1, 0);

		case Direction::LeftDown:
			return _Point(-1, 1);
			
		case Direction::Down:
			return _Point(0, 1);
			
		case Direction::RightDown:
			return _Point(1, 1);
			
		}
		return _Point(0, 0);
	}

	const std::u32string ToString(Action action);

	const std::u32string ToString(Direction dir);

	const std::u32string ToString(Step step);

	const std::u32string ToString(TeamType team);

	const char ToChar(TileType tile);
}