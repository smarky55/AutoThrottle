// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include "Performance.h"
#include "PID.h"

#include <string>

using XPLMDataRef = void*;
using XPLMFlightLoopID = void*;

class AutoThrottlePlugin {
public:
	AutoThrottlePlugin();
	~AutoThrottlePlugin();

	AutoThrottlePlugin(const AutoThrottlePlugin&) = delete;
	AutoThrottlePlugin& operator=(const AutoThrottlePlugin&) = delete;

	static float flightLoopCallback(
		float timeSinceLastCall,
		float timeSinceLastFlightLoop,
		int counter,
		void* refcon);

	void loadPerformance(const std::string& path);

	void setupDatarefs();

	void setupFlightLoop();

	void activateAutoThrottle();

	void deactivateAutoThrottle();

	Performance& performance();
	const Performance& performance() const;

	PID& pid();
	const PID& pid() const;

	bool isEnabled() const;
	void isEnabled(bool enabled);

private:

	// Datarefs
	XPLMDataRef m_timeDref;
	XPLMDataRef m_throttleDref;
	XPLMDataRef m_target1Dref;

	XPLMDataRef m_joyAxisAssignments;
	int m_throttleAxisIndex;

	Performance m_performance;
	PID m_primaryPID;

	bool m_isEnabled;
	int m_currentMode;

	XPLMFlightLoopID m_flightLoop;
	float m_lastTime;
	float m_throttle;
};

