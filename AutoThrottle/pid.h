// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0.If a copy of the MPL was not distributed with this
// file, You can obtain one at http ://mozilla.org/MPL/2.0/.

#pragma once

#include <algorithm>

class PID {
public:
	PID();
	~PID();

	float update(float processValue, float dt);

	void setTarget(float target);

	void setGains(float p, float i, float d);
	void getGains(float* p, float* i, float* d) {
		*p = kP;
		*i = kI;
		*d = kD;
	}

	void resetIntegral();

	void setIntegralClamp(bool enable, float clamp);
	void setIntegralClamp(bool enable);

private:
	float setPoint;
	float kP;
	float kI;
	float kD;

	float integral = 0.0;
	bool clampIntegral = true;
	float iClamp = 1.0;
	float last_error = 0.0;
};
