#pragma once
#include "FieldDrawer.h"
#include "Field.h"
#include "Team.h"

// TODO: 領域ポイントの処理
class Game
{
private:
	Field _field;
	FieldDrawer _field_drawer;
	Array<std::shared_ptr<Team>> _teams;

	Array<Point> _arrives;
	Grid<bool> _status;
private:
	void dfsAreaPoint(Point pos, TileType tile);

	GameInfo getGameInfo() const;
	std::map<TeamType, Array<Agent>> getAgentMap() const;
	Array<Agent> getAgents() const;

public:
	/// <summary>
	/// 指定のタイルで囲まれた領域の得点を計算します
	/// </summary>
	/// <param name="tile">どのタイルで囲まれているか</param>
	/// <returns>領域ポイント</returns>
	int GetAreaPoint(TileType tile);

	void Update();
	void Draw() const;

public:
	Game(const Field &field, std::shared_ptr<Team> team_a, std::shared_ptr<Team> team_b);
	virtual ~Game();
};
