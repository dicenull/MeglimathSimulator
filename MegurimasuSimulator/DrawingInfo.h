#pragma once
#include <Siv3D.hpp>

class DrawingInfo
{
protected:
	static constexpr Size cellSize = Size(32, 32);
	static constexpr Point fieldOrigin = Point(16, 64);
	static constexpr Point statOrigin = Point(410, 64);
};
