// This Source Code Form is subject to the terms of the Mozilla Public
// License, v.2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

struct Rect {
	int left;
	int top;
	int right;
	int bottom;
};

Rect operator+(const Rect& lhs, const Rect& rhs);
Rect operator-(const Rect& lhs, const Rect& rhs);

float mToFt(float m);

float getIsaDev(float altFeet, float tempC);

float clamp(float var, float min, float max);