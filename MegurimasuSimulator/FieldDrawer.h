#pragma once
#include "Field.h"
#include <Siv3D.hpp>

class FieldDrawer
{
private:
	Point _origin;

	/// <summary>
	/// ƒZƒ‹ˆê‚Â“–‚½‚è‚Ì‘å‚«‚³
	/// </summary>
	const Size _c_size = Size(32, 32);

public:
	void SetOrigin(Point origin);
	Point GetOrigin() const;

	void Draw(const Field & field) const;

public:
	FieldDrawer();
	~FieldDrawer();

	void operator=(const FieldDrawer& other);
};

