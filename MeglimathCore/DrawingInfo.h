#pragma once
#include <Siv3D.hpp>

class DrawingInfo
{
protected:
	/// <summary>
	/// セル一つ当たりの描画する大きさ
	/// </summary>
	static constexpr Size cellSize = Size(32, 32);

	/// <summary>
	/// フィールドを描画する際の左上の座標
	/// </summary>
	static constexpr Point fieldOrigin = Point(16, 64);

	/// <summary>
	/// ステータス情報を描画する際の左上の座標
	/// </summary>
	static constexpr Point statOrigin = Point(410, 64);
};
