#pragma once
#include<assert.h>
#include<vector>
#include<string>
#include<memory>
enum TeamType {
	A = 0,
	B = 1
};

enum class TileType {
	A = TeamType::A,
	B = TeamType::B,
	None
};
template <class Component = int>
class _Point {
public:
	Component x;
	Component y;
	constexpr _Point(Component x = 0, Component y = 0) :x(x), y(y) {}

	_Point(const char * str) {
		assert(str[0] == '(');
		assert(str[std::strlen(str) - 1] == ')');
		auto comma = std::strchr(str, ',');
		int _x, _y;
		sscanf_s(str + 1, "%d", &_x);
		sscanf_s(comma + 1, "%d", &_y);
		x = _x; y = _y;
	}
	template <class Comp = int>
	_Point operator+(const _Point<Comp>& r)const {
		return _Point(x + r.x, y + r.y);
	}
	template <class Comp = int>
	_Point operator-(const _Point<Comp>& r)const {
		return _Point(x - r.x, y - r.y);
	}
	template <class Comp = int>
	_Point& operator+=(const _Point<Comp>& r) {
		*this = *this + r;
		return *this;
	}
	template <class Comp = int>
	_Point& operator-=(const _Point<Comp>& r) {
		*this = *this - r;
		return *this;
	}
	template <class Comp = int>
	constexpr bool operator==(const _Point<Comp>& r) {
		return x == r.x && y == r.y;
	}
	template <class Comp = int>
	bool operator<(const _Point<Comp>& r) {
		return x < r.x || (x == r.x && y < r.y);
	}
	template <class Comp = int>
	operator _Point<Comp>() {
		return _Point<Comp>{(Comp)this->x, (Comp)this->y};
	}
	template <class Comp = int>
	operator const _Point<Comp>()const {
		return _Point<Comp>{(Comp)this->x, (Comp)this->y};
	}
};

typedef _Point<size_t> _Size;

template <class Component>
class _Grid {
public:
	_Size _size;
	using Comp = std::array<Component, 12 * 12>;
	Comp component;
	//auto& operator[](size_t index) {
	//	return component[index * 12];
	//}
	//constexpr const auto& operator[](size_t index)const {
	//	return component[index * 12];
	//}
	auto& operator[](_Size index) {
		return component[index.y * 12 + index.x];
	}
	constexpr const auto& operator[](_Size index)const {
		return component[index.y * 12 + index.x];
	}
	constexpr const size_t width()const { return _size.x; }
	constexpr const size_t height() const { return _size.y; }
	constexpr const _Size size()const { return _size; }

	_Grid() :_Grid(_Size(0, 0)) {}
	_Grid(_Size size) :_size(size) {}

	_Grid(const _Grid& g) : _size(g.size()), component(g.component) {}
	_Grid& operator=(const _Grid<Component>& g) {
		_size = g.size();
		component = g.component;
		return *this;
	}
};

