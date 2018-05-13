#pragma once
#include"GameLogic/GameLogic.h"
#include<Siv3D.hpp>
namespace LogicUtil {
	static Point toS3dPoint(const _Point<> &_p) {
		return Point{ _p.x,_p.y };
	}
	static HashTable<TeamType, Array<Agent>> toS3dHashTable(const std::unordered_map<TeamType, std::vector<Agent>> &_map) {
		HashTable<TeamType, Array<Agent>> ret;
		for (auto k : _map)ret[k.first] = k.second;
		return ret;
	}
	static Array<Agent> toS3dArray(const std::vector<Agent> &_vec) {
		Array<Agent> ret;
		for (auto a : _vec)ret.push_back(a);
		return ret;
	}
	static std::unordered_map<TeamType, Think> fromS3dHashTable(const HashTable<TeamType, Think>& _hash) {
		std::unordered_map<TeamType, Think> ret{};
		for (auto t = _hash.cbegin(); t != _hash.cend(); t++)ret[t.key()] = t.value();
		return ret;
	}
}