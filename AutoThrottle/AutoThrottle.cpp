// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0.If a copy of the MPL was not distributed with this
// file, You can obtain one at http ://mozilla.org/MPL/2.0/.


#ifdef IBM

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>

BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
) {
	switch (ul_reason_for_call) {
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

#endif // IBM

#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "XPLM/XPLMDataAccess.h"
#include "XPLM/XPLMDisplay.h"
#include "XPLM/XPLMGraphics.h"
#include "XPLM/XPLMMenus.h"
#include "XPLM/XPLMPlugin.h"
#include "XPLM/XPLMProcessing.h"
#include "XPLM/XPLMUtilities.h"

#if LIN
#include <GL/gl.h>
#elif __GNUC__
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

#include "cereal/archives/json.hpp"

#include "perf.h"
#include "pid.h"

// Window handle
static XPLMWindowID window;

// Callbacks
void draw_hello_world(XPLMWindowID in_window_id, void* in_refcon);
int dummy_mouse_handler(XPLMWindowID in_window_id, int x, int y, int is_down, void* in_refcon) { return 0; }
XPLMCursorStatus dummy_cursor_status_handler(XPLMWindowID in_window_id, int x, int y, void* in_refcon) { return xplm_CursorDefault; }
int dummy_wheel_handler(XPLMWindowID in_window_id, int x, int y, int wheel, int clicks, void* in_refcon) { return 0; }
void dummy_key_handler(XPLMWindowID in_window_id, char key, XPLMKeyFlags flags, char virtual_key, void* in_refcon, int losing_focus) {}

float autothrottleFlightLoopCallback(float inTimeSinceLastCall, float inTimeSinceLastFlightLoop, int inCounter, void* inRefcon);

void MenuHandler(void* inMenuRef, void* inItemRef);

XPLMDataRef timeDref;
XPLMDataRef throttleDref;
XPLMDataRef trqDref;

XPLMFlightLoopID flightLoop;

Performance perf;

bool bAutoThrottleEnabled = false;
int perfMode = -1;
PID trqPid;

PLUGIN_API int XPluginStart(char* outName, char* outSig, char* outDesc) {
	strcpy_s(outName, 19, "AutoThrottlePlugin");
	strcpy_s(outSig, 32, "smarky55.tbm.autothrottleplugin");
	strcpy_s(outDesc, 57, "An autothrottle implementation for the Hotstart TBM 900.");

	XPLMEnableFeature("XPLM_USE_NATIVE_WIDGET_WINDOWS", true);
	
	XPLMDebugString("[AutoThrottle] Init\n");
	
// Test window
#ifdef _DEBUG

	XPLMCreateWindow_t params;
	params.structSize = sizeof(params);
	params.visible = 1;
	params.drawWindowFunc = draw_hello_world;
	params.handleMouseWheelFunc = dummy_wheel_handler;
	params.handleMouseClickFunc = dummy_mouse_handler;
	params.handleRightClickFunc = dummy_mouse_handler;
	params.handleCursorFunc = dummy_cursor_status_handler;
	params.handleKeyFunc = dummy_key_handler;
	params.refcon = NULL;
	params.layer = xplm_WindowLayerFloatingWindows;
	params.decorateAsFloatingWindow = xplm_WindowDecorationRoundRectangle;

	int left, bottom, right, top;
	XPLMGetScreenBoundsGlobal(&left, &top, &right, &bottom);
	params.left = left + 50;
	params.bottom = bottom + 150;
	params.right = params.left + 200;
	params.top = params.bottom + 200;

	window = XPLMCreateWindowEx(&params);
	XPLMSetWindowPositioningMode(window, xplm_WindowPositionFree, -1);
	XPLMSetWindowResizingLimits(window, 200, 200, 300, 300);
	XPLMSetWindowTitle(window, "Test window");

#endif // DEBUG

#ifdef _DEBUG
	Performance p;
	p.test();

	std::stringstream ss;

	ss << p.tables["TST"].getValue(2.5, 35) << std::endl;

	cereal::JSONOutputArchive output(ss);
	output(cereal::make_nvp("testperf", p));
	ss << std::endl;

	XPLMDebugString(ss.str().c_str());

#endif // _DEBUG

	char sysPath[512];
	XPLMGetSystemPath(sysPath);
	std::string perfPath = sysPath;
	perfPath.append("Resources\\plugins\\AutoThrottle\\tbm9.perf");
	XPLMDebugString(perfPath.c_str());

	std::ifstream perfFile(perfPath);


	try {
		cereal::JSONInputArchive perfArchive(perfFile);
		perfArchive(perf);
	}
	catch (const std::exception& e) {
		XPLMDebugString(e.what());
	}

	perfFile.close();
	
	timeDref = XPLMFindDataRef("sim/time/total_running_time_sec");
	throttleDref = XPLMFindDataRef("sim/cockpit2/engine/actuators/throttle_ratio_all");
	trqDref = XPLMFindDataRef("sim/flightmodel/engine/ENGN_TRQ");

	XPLMMenuID myMenu;
	int myMenuSubItem;

	myMenuSubItem = XPLMAppendMenuItem(XPLMFindPluginsMenu(), "AutoThrottle", NULL, 0);
	myMenu = XPLMCreateMenu("AutoThrottle", XPLMFindPluginsMenu(), myMenuSubItem, MenuHandler, NULL);
	XPLMAppendMenuItem(myMenu, "Test", reinterpret_cast<void*>(1), 0);
	
	XPLMCreateFlightLoop_t createFlightLoop;
	createFlightLoop.structSize = sizeof(XPLMCreateFlightLoop_t);
	createFlightLoop.phase = xplm_FlightLoop_Phase_BeforeFlightModel;
	createFlightLoop.callbackFunc = autothrottleFlightLoopCallback;

	flightLoop = XPLMCreateFlightLoop(&createFlightLoop);

	trqPid.setGains(1.0f, 0.0f, 0.0f);

	return window != NULL;
}

PLUGIN_API void XPluginStop(void) {

#ifdef _DEBUG

	XPLMDestroyWindow(window);
	window = NULL;

#endif // _DEBUG
}

PLUGIN_API int XPluginEnable(void) { return 1; }
PLUGIN_API void XPluginDisable(void) {}
PLUGIN_API void XPluginReceiveMessage(XPLMPluginID inFrom, int inMsg, void* inParam) {}

void draw_hello_world(XPLMWindowID inID, void* inRefcon) {
	XPLMSetGraphicsState(0, 0, 0, 0, 1, 1, 0);

	int l, t, r, b;
	XPLMGetWindowGeometry(inID, &l, &t, &r, &b);
	float col_white[] = { 1.0, 1.0, 1.0 };
	XPLMDrawString(col_white, l + 10, t - 20, const_cast<char*>("Testing!"), NULL, xplmFont_Proportional);
	std::stringstream ss;
	ss << XPLMGetDataf(timeDref);
	XPLMDrawString(col_white, l + 10, t - 40, const_cast<char*>(ss.str().c_str()), NULL, xplmFont_Proportional);
}

void MenuHandler(void* inMenuRef, void* inItemRef) {
	switch (reinterpret_cast<int>(inItemRef)) {
	case 1: // Testing
		if (bAutoThrottleEnabled) {
			bAutoThrottleEnabled = false;
			XPLMScheduleFlightLoop(flightLoop, 0.0f, false);
		}
		else {
			bAutoThrottleEnabled = true;
			trqPid.setTarget(1500.0f);
			XPLMScheduleFlightLoop(flightLoop, -1.0, false);
		}
		break;
	default:
		break;
	}
}

float autothrottleFlightLoopCallback(float inTimeSinceLastCall, float inTimeSinceLastFlightLoop, int inCounter, void* inRefcon) {
	static float lastTime = XPLMGetDataf(timeDref);
	float currentTime = XPLMGetDataf(timeDref);
	float dt = currentTime - lastTime;
	lastTime = currentTime;

	if (perfMode >= 0) {
		trqPid.setTarget(perf.tables[perf.modes[perfMode]].getValue());
	}

	float currentTrq;
	XPLMGetDatavf(trqDref, &currentTrq, 0, 1);
	float throttle = std::min(1.0f, std::max(0.35f, XPLMGetDataf(throttleDref) + (0.001f * trqPid.update(currentTrq, dt))));
	XPLMSetDataf(throttleDref, throttle);
	
	return -1.0;
}

void setupWidgets() {

}