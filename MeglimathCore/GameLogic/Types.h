#pragma once
#include<assert.h>
#include<vector>
#include<string.h>
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
	constexpr _Point(Component x = 0, Component y = 0) :x(x), y(y) {}

	_Point(const char * str) {
		assert(str[0] == '(');
		assert(str[std::strlen(str)-1] == ')');
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
	bool operator==(const _Point<Comp>& r) {
		return x == r.x && y == r.y;
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

	_Grid() :_Grid(_Size(0, 0)) {}
	_Grid(_Size size) :_size(size) { 
		for (int i=0; i < size.y; i++) {
			std::vector<Component> v;
			for (int j=0; j < size.x; j++) {
				v.emplace_back();
			}
			component.emplace_back(v);
		}
	}
	_Grid(const _Grid& g) :component(g.component), _size(g._size) {};
};

