// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0.If a copy of the MPL was not distributed with this
// file, You can obtain one at http ://mozilla.org/MPL/2.0/.

#include "util.h"

float mToFt(float m) {
	return m * 3.28084f;
}

float getIsaDev(float altFeet, float tempC) {
	const float ISA_SL_TEMP = 15.0f; // degrees Celsius
	const float ISA_TROPOPAUSE_HEIGHT = 36089; // ft MSL
	const float ISA_LAPSE_RATE = 1.98f; // degrees Celsius per 1000ft

	if (altFeet < ISA_TROPOPAUSE_HEIGHT) {
		return ISA_SL_TEMP - ISA_LAPSE_RATE * (altFeet / 1000.0f);
	} else {
		return -56.5;
	}
}
