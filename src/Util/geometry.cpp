// This Source Code Form is subject to the terms of the Mozilla Public
// License, v.2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include <AutoThrottle/Util/geometry.h>
#include <algorithm>

Point operator+(const Point& lhs, const Point& rhs)
{
	return Point{ lhs.x + rhs.x, lhs.y + rhs.y };
}

Point operator-(const Point& lhs, const Point& rhs)
{
	return Point{ lhs.x - rhs.x, lhs.y - rhs.y };
}

Rect minimumRect(const Rect& a, const Rect& b)
{
	Rect ret;
	ret.left = std::min(a.left, b.left);
	ret.top = std::max(a.top, b.top);
	ret.right = std::max(a.right, b.right);
	ret.bottom = std::min(a.bottom, b.bottom);
	return ret;
}

Rect operator+(const Rect& lhs, const Rect& rhs)
{
	Rect ret(
		lhs.left + rhs.left,
		lhs.top + rhs.top,
		lhs.right + rhs.right,
		lhs.bottom + rhs.bottom
	);
	return ret;
}

Rect operator-(const Rect& lhs, const Rect& rhs)
{
	Rect ret(
		lhs.left - rhs.left,
		lhs.top - rhs.top,
		lhs.right - rhs.right,
		lhs.bottom - rhs.bottom
	);
	return ret;
}

Rect::Rect()
	: left(0), top(0), right(0), bottom(0)
{
}

Rect::Rect(int left, int top, int right, int bottom)
	: left(left), top(top), right(right), bottom(bottom)
{
}

Rect::Rect(Point topLeft, Point bottomRight)
	: left(topLeft.x), top(topLeft.y), right(bottomRight.x), bottom(bottomRight.y)
{
}

bool Rect::contains(Point point) const
{
	return (left <= point.x && point.x <= right) && (bottom <= point.y && point.y <= top);
}

Rect& Rect::operator+=(const Rect& rhs)
{
	left += rhs.left;
	top += rhs.top;
	right += rhs.right;
	bottom += rhs.bottom;
	return *this;
}
