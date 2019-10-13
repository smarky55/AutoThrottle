// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "AutoThrottlePlugin.h"

#include <cereal/archives/json.hpp>

#include <XPLM/XPLMDataAccess.h>
#include <XPLM/XPLMMenus.h>
#include <XPLM/XPLMProcessing.h>
#include <XPLM/XPLMUtilities.h>

#include <array>
#include <fstream>

#include "util.h"

AutoThrottlePlugin::AutoThrottlePlugin()
	: m_timeDref(nullptr),
	m_throttleDref(nullptr),
	m_target1Dref(nullptr),
	m_joyAxisAssignments(nullptr),
	m_throttleAxisIndex(-1),
	m_performance(),
	m_primaryPID(),
	m_isEnabled(false),
	m_currentMode(-1),
	m_flightLoop(nullptr),
	m_lastTime(0),
	m_throttle(0),
	m_menu("AutoThrottle", XPLMFindPluginsMenu())
{
}

AutoThrottlePlugin::~AutoThrottlePlugin()
{
	if (m_flightLoop) {
		XPLMDestroyFlightLoop(m_flightLoop);
	}
}

float AutoThrottlePlugin::flightLoopCallback(float timeSinceLastCall, float timeSinceLastFlightLoop, int counter, void* refcon)
{
	if (!refcon) {
		return 0.0f;
	}
	AutoThrottlePlugin* at = static_cast<AutoThrottlePlugin*>(refcon);

	float currentTime = XPLMGetDataf(at->m_timeDref);
	float dt = currentTime - at->m_lastTime;
	at->m_lastTime = currentTime;
	
	// Autothrottle just (re)activated, assume values are bad and skip this call
	if (timeSinceLastCall > 0.1) {
		at->m_throttle = XPLMGetDataf(at->m_throttleDref);
		return -1.0f;
	}

	if (at->m_currentMode >= 0) {
		at->m_primaryPID.setTarget(at->m_performance.tables[at->m_performance.modes[at->m_currentMode]].getValue());
	}

	float processVar1;
	XPLMGetDatavf(at->m_target1Dref, &processVar1, 0, 1);
	float pidSpeed = at->m_primaryPID.update(processVar1, dt) * dt;
	at->m_throttle = clamp(at->m_throttle + 0.001f * pidSpeed, .35f, 1.0f);
	XPLMSetDataf(at->m_throttleDref, at->m_throttle);

	return -1.0f;
}

void AutoThrottlePlugin::loadPerformance(const std::string& path)
{
	std::ifstream performanceFile(path);

	try {
		cereal::JSONInputArchive performanceArchive(performanceFile);
		performanceArchive(m_performance);
	} catch (const std::exception& e) {
		XPLMDebugString(e.what());
	}
}

void AutoThrottlePlugin::setupDatarefs()
{
	m_timeDref = XPLMFindDataRef("sim/time/total_running_time_sec");
	m_throttleDref = XPLMFindDataRef("sim/cockpit2/engine/actuators/throttle_ratio_all");
	m_target1Dref = XPLMFindDataRef("sim/flightmodel/engine/ENGN_TRQ"); // TODO: set this to perf's target

	m_joyAxisAssignments = XPLMFindDataRef("sim/joystick/joystick_axis_assignments");
}

void AutoThrottlePlugin::setupFlightLoop()
{
	XPLMCreateFlightLoop_t createFlightLoop;
	createFlightLoop.structSize = sizeof(XPLMCreateFlightLoop_t);
	createFlightLoop.phase = xplm_FlightLoop_Phase_BeforeFlightModel;
	createFlightLoop.callbackFunc = AutoThrottlePlugin::flightLoopCallback;
	createFlightLoop.refcon = this;

	m_flightLoop = XPLMCreateFlightLoop(&createFlightLoop);
}

void AutoThrottlePlugin::activateAutoThrottle()
{
	std::array<int, 32> joyAxes = { 0 };
	int nAxes = XPLMGetDatavi(m_joyAxisAssignments, joyAxes.data(), 0, 32);
	for (int i = 0; i < nAxes; i++) {
		if (joyAxes[i] == 4) {
			m_throttleAxisIndex = i;
			break;
		}
	}
	if (m_throttleAxisIndex < 0) {
		XPLMDebugString("[Autothrottle] No unified throttle axis bound!\n");
	} else {
		int unbindAxis = 0;
		XPLMSetDatavi(m_joyAxisAssignments, &unbindAxis, m_throttleAxisIndex, 1);
		m_isEnabled = true;
		XPLMScheduleFlightLoop(m_flightLoop, -1.0f, false);
	}
}

void AutoThrottlePlugin::deactivateAutoThrottle()
{
	if (m_throttleAxisIndex >= 0) {
		int throttleAxis = 4;
		XPLMSetDatavi(m_joyAxisAssignments, &throttleAxis, m_throttleAxisIndex, 1);
		m_throttleAxisIndex = -1;
	}
	m_isEnabled = false;
	XPLMScheduleFlightLoop(m_flightLoop, 0.0f, false);
}

Performance& AutoThrottlePlugin::performance()
{
	return m_performance;
}

const Performance& AutoThrottlePlugin::performance() const
{
	return m_performance;
}

PID& AutoThrottlePlugin::pid()
{
	return m_primaryPID;
}

const PID& AutoThrottlePlugin::pid() const
{
	return m_primaryPID;
}

MenuItem& AutoThrottlePlugin::menu()
{
	return m_menu;
}

bool AutoThrottlePlugin::isEnabled() const
{
	return m_isEnabled;
}

void AutoThrottlePlugin::isEnabled(bool enabled)
{
	m_isEnabled = enabled;
}
