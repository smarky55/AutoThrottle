// This Source Code Form is subject to the terms of the Mozilla Public
// License, v.2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

struct Point {
	int x;
	int y;
};

Point operator+(const Point& lhs, const Point& rhs);
Point operator-(const Point& lhs, const Point& rhs);

class Rect {
public:
	Rect(); // Zeros
	Rect(int left, int top, int right, int bottom);
	Rect(Point topLeft, Point bottomRight);

	int left;
	int top;
	int right;
	int bottom;

	bool contains(Point point) const;

	Rect& operator+=(const Rect& rhs);
};

// The smallest rect that contains a & b
Rect minimumRect(const Rect& a, const Rect& b);

Rect operator+(const Rect& lhs, const Rect& rhs);
Rect operator-(const Rect& lhs, const Rect& rhs);