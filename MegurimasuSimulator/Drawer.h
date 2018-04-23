#pragma once
#include "Field.h"
#include "DrawingInfo.h"
#include "Think.h"
#include <Siv3D.hpp>

class Drawer : public DrawingInfo
{
public:
	/// <summary>
	/// フィールドを描画する
	/// </summary>
	/// <param name="field">描画するフィールド</param>
	void DrawField(const Field & field, Array<Agent> agents) const;

	/// <summary>
	/// すべてのエージェントを描画する
	/// </summary>
	/// <param name="agents">エージェントリスト</param>
	void DrawAgents(std::map<TeamType, Array<Agent>> agents) const;

	/// <summary>
	/// エージェントの行動やターン数を表示する
	/// </summary>
	/// <param name="thinks">すべてのエージェントの行動</param>
	/// <param name="turn">残りターン数</param>
	/// <param name="field">チームの合計得点を描画するためのフィールド情報</param>
	void DrawStatus(const std::map<TeamType, Think> & thinks, const Field & field, int turn) const;

public:
	Drawer();
	~Drawer();

	void operator=(const Drawer& other);
};

