#pragma once
#include<string>
#include<assert.h>
#include<vector>
enum class TileType
{
	A, B, None
};

enum class TeamType
{
	A, B
};

template <class Component = int>
class _Point {
public:
	Component x;
	Component y;
	constexpr _Point(Component x=0, Component y=0):x(x),y(y){}

	_Point(std::string str) {
		assert(str[0] == '(');
		assert(str[str.length()] == ')');
		auto comma = str.find(',');
		assert(comma < str.length() - 2);
		sscanf_s(&str[1], "%d", &x);
		sscanf_s(&str[comma + 1], "%d", &y);
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
	_Point& operator+=(const _Point<Comp>& r){
		*this = *this + r;
		return *this;
	}
	template <class Comp = int>
	_Point& operator-=(const _Point<Comp>& r) {
		*this = *this - r;
		return *this;
	}
	template <class Comp = int>
	bool operator==(const _Point<Comp>& r) {
		return x == r.x && y == r.y;
	}
	template <class Comp = int>
	operator _Point<Comp>() {
		return _Point<Comp>{(Comp)this->x, (Comp)this->y};
	}
};

typedef _Point<size_t> _Size;

template <class Component>
class _Grid{
	std::vector<std::vector<Component>> component;
	_Size _size;
public:
	std::vector<Component>& operator[](size_t index) {
		return component[index];
	}
	const std::vector<Component>& operator[](size_t index)const {
		return component[index];
	}

	const size_t width()const {
		return _size.x;
	}

	const size_t height() const {
		return _size.y;
	}

	const _Size size()const {
		return _size;
	}

	_Grid():_Grid(_Size(0,0)){}
	_Grid(_Size size):_size(size){}
	_Grid(const _Grid& g) :component(g.component), _size(g._size) {};
};

static std::vector<int> step(size_t n) {
	std::vector<int> ret;
	ret.reserve(n);
	for (int i = 0; i < n; i++)ret[n] = i;
	return ret;
}