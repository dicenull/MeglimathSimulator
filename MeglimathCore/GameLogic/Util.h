#pragma once
#include<vector>
static std::vector<int> step(size_t n) {
	std::vector<int> ret;
	for (int i = 0; i < n; i++)ret.push_back(i);
	return ret;
}
