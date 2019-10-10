// This Source Code Form is subject to the terms of the Mozilla Public
// License, v.2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "ap_core.h"

#include <XPLM/XPLMDataAccess.h>
#include <XPLM/XPLMProcessing.h>
#include <XPLM/XPLMUtilities.h>

#include "util.h"

// GLOBALS

// Datarefs

XPLMDataRef timeDref;
XPLMDataRef throttleDref;
XPLMDataRef trqDref;

// Autothrottle properties

Performance perf;

bool bAutoThrottleEnabled = false;
int perfMode = -1;
PID trqPid;

// Flight loop

XPLMFlightLoopID flightLoop;

XPLMDataRef dJoyAxisAssignments;
int throttleAxisIndex = -1;


// FUNCTIONS

float autothrottleFlightLoopCallback(float inTimeSinceLastCall, float inTimeSinceLastFlightLoop, int inCounter, void* inRefcon) {
	static float lastTime = XPLMGetDataf(timeDref);
	static float throttle = XPLMGetDataf(throttleDref);
	float currentTime = XPLMGetDataf(timeDref);
	float dt = currentTime - lastTime;
	lastTime = currentTime;

	// Autothrottle just (re)activated, assume throttle and dt are bad
	if (inTimeSinceLastCall > 0.1) {
		throttle = XPLMGetDataf(throttleDref);
		dt = 0.001f;
	}

	if (perfMode >= 0) {
		trqPid.setTarget(perf.tables[perf.modes[perfMode]].getValue());
	}

	float currentTrq;
	XPLMGetDatavf(trqDref, &currentTrq, 0, 1);
	float pidspeed = trqPid.update(currentTrq, dt) * dt;
	throttle = clamp(throttle + 0.001f * pidspeed, .35f, 1.0f);
	XPLMSetDataf(throttleDref, throttle);

	return -1.0;
}

void setupFlightLoop() {
	XPLMCreateFlightLoop_t createFlightLoop;
	createFlightLoop.structSize = sizeof(XPLMCreateFlightLoop_t);
	createFlightLoop.phase = xplm_FlightLoop_Phase_BeforeFlightModel;
	createFlightLoop.callbackFunc = autothrottleFlightLoopCallback;

	flightLoop = XPLMCreateFlightLoop(&createFlightLoop);
}

void activateAutoThrottle() {
	int joyAxes[32] = { 0 };
	int nAxes = XPLMGetDatavi(dJoyAxisAssignments, joyAxes, 0, 32);
	for (int i = 0; i < nAxes; i++) {
		if (joyAxes[i] == 4) {
			throttleAxisIndex = i;
			break;
		}
	}
	if (throttleAxisIndex < 0) {
		XPLMDebugString("[Autothrottle] No unified throttle axis bound!\n");
	}
	else {
		int unbindAxis = 0;
		XPLMSetDatavi(dJoyAxisAssignments, &unbindAxis, throttleAxisIndex, 1);
		bAutoThrottleEnabled = true;
		XPLMScheduleFlightLoop(flightLoop, -1.0f, false);
	}

}

void deactivateAutoThrottle() {
	if (throttleAxisIndex >= 0) {
		int throttleAxis = 4;
		XPLMSetDatavi(dJoyAxisAssignments, &throttleAxis, throttleAxisIndex, 1);
		throttleAxisIndex = -1;
	}
	bAutoThrottleEnabled = false;
	XPLMScheduleFlightLoop(flightLoop, 0.0f, false);
}
