#pragma once
struct GamePoints {
	int area, tile;
	int getTotal() const {
		return area + tile;
	}
};

