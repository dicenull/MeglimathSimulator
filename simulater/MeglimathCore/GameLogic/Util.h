#pragma once
#include<vector>
#include<array>

template<int n>
inline std::array<int, n> step() {
	auto a = std::array<int, n>();
	std::iota(a.begin(), a.end(), 0);
	return a;
}

inline std::vector<int> step(size_t n) {
	auto v = std::vector<int>(n);
	std::iota(v.begin(), v.end(), 0);
	return v;
}