#pragma once
class GamePoints
{
private:
	int _area, _tile;

public:
	int GetArea() const;
	int GetTile() const;
	int GetTotal() const;

public:
	GamePoints();
	GamePoints(int area, int tile);
	~GamePoints();
};

