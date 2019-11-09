// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0.If a copy of the MPL was not distributed with this
// file, You can obtain one at http ://mozilla.org/MPL/2.0/.

#include <algorithm>

#include <AutoThrottle/PID.h>

PID::PID() {
	setPoint = 0.0;
	kP = 1.0;
	kI = 0.0;
	kD = 0.0;
}

PID::~PID() {}

float PID::update(float processValue, float dt) {
	float error, p, i, d;
	error = setPoint - processValue;

	p = kP * error;

	integral = integral + error * dt;
	if (clampIntegral) {
		integral = std::max(std::min(integral, iClamp), -iClamp);
	}
	i = integral;

	d = (error - last_error) / dt;
	last_error = error;

	return p + i + d;
}

void PID::setTarget(float target) {
	setPoint = target;
}

void PID::setGains(float p, float i, float d) {
	kP = p;
	kI = i;
	kD = d;
}

void PID::resetIntegral() {
	integral = 0.0;
}

void PID::setIntegralClamp(bool enable, float clamp) {
	clampIntegral = enable;
	if (clamp > 0) {
		iClamp = clamp;
	}
}

void PID::setIntegralClamp(bool enable) {
	clampIntegral = enable;
}
