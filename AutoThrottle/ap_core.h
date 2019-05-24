// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0.If a copy of the MPL was not distributed with this
// file, You can obtain one at http ://mozilla.org/MPL/2.0/.

#pragma once

#include <XPLM/XPLMDataAccess.h>
#include <XPLM/XPLMProcessing.h>
#include <XPLM/XPLMUtilities.h>

#include "perf.h"
#include "pid.h"


// Datarefs

extern XPLMDataRef timeDref;
extern XPLMDataRef throttleDref;
extern XPLMDataRef trqDref;

// Autothrottle properties

extern Performance perf;

extern bool bAutoThrottleEnabled;
extern int perfMode;
extern PID trqPid;

// Flight loop

extern XPLMFlightLoopID flightLoop;

extern XPLMDataRef dJoyAxisAssignments;
extern int throttleAxisIndex;

float autothrottleFlightLoopCallback(float inTimeSinceLastCall, float inTimeSinceLastFlightLoop, int inCounter, void* inRefcon);

void setupFlightLoop();

void activateAutoThrottle();

void deactivateAutoThrottle();