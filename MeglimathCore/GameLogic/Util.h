#pragma once
#include<vector>
#include<array>

template<int n>
inline const std::array<int, n> &step() {
	auto a = std::array<int, n>();
	std::iota(a.begin(), a.end());
	return a;
}

inline const std::vector<int> &step(size_t n) {
	auto v = std::vector<int>(n);
	std::iota(v.begin(), v.end(), n);
	return v;
}