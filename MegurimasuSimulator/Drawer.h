#pragma once
#include "GameLogic/Field.h"
#include "DrawingInfo.h"
#include "GameLogic/Think.h"
#include <Siv3D.hpp>

class Drawer : public DrawingInfo
{
public:
	/// <summary>
	/// フィールドを描画する
	/// </summary>
	/// <param name="field">描画するフィールド</param>
	void DrawField(const Field & field) const;

	/// <summary>
	/// すべてのエージェントを描画する
	/// </summary>
	/// <param name="agents">エージェントリスト</param>
	void DrawAgents(HashTable<TeamType, Array<Agent>> agents) const;

	/// <summary>
	/// エージェントの行動やターン数を表示する
	/// </summary>
	/// <param name="thinks">すべてのエージェントの行動</param>
	/// <param name="turn">残りターン数</param>
	/// <param name="field">チームの合計得点を描画するためのフィールド情報</param>
	void DrawStatus(const HashTable<TeamType, Think> & thinks, const Field & field, int turn) const;

public:
	Drawer();
	~Drawer();
};

