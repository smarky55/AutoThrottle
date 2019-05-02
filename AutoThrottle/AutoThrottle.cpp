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

#include <string>
#include <sstream>

#include "XPLM/XPLMDisplay.h"
#include "XPLM/XPLMGraphics.h"
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

// Window handle
static XPLMWindowID window;

// Callbacks
void draw_hello_world(XPLMWindowID in_window_id, void* in_refcon);
int dummy_mouse_handler(XPLMWindowID in_window_id, int x, int y, int is_down, void* in_refcon) { return 0; }
XPLMCursorStatus dummy_cursor_status_handler(XPLMWindowID in_window_id, int x, int y, void* in_refcon) { return xplm_CursorDefault; }
int dummy_wheel_handler(XPLMWindowID in_window_id, int x, int y, int wheel, int clicks, void* in_refcon) { return 0; }
void dummy_key_handler(XPLMWindowID in_window_id, char key, XPLMKeyFlags flags, char virtual_key, void* in_refcon, int losing_focus) {}

PLUGIN_API int XPluginStart(char* outName, char* outSig, char* outDesc) {
	strcpy_s(outName, 19, "AutoThrottlePlugin");
	strcpy_s(outSig, 32, "smarky55.tbm.autothrottleplugin");
	strcpy_s(outDesc, 57, "An autothrottle implementation for the Hotstart TBM 900.");
	
	XPLMDebugString("[AutoThrottle] Init\n");
	
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

#ifdef _DEBUG
	PerfTable p;
	p.testPerf();

	std::stringstream ss;

	cereal::JSONOutputArchive output(ss);
	output(cereal::make_nvp("testperf", p));
	ss << std::endl;

	XPLMDebugString(ss.str().c_str());

#endif // _DEBUG


	return window != NULL;
}

PLUGIN_API void XPluginStop(void) {
	XPLMDestroyWindow(window);
	window = NULL;
}

PLUGIN_API int XPluginEnable(void) { return 1; }
PLUGIN_API void XPluginDisable(void) {}
PLUGIN_API void XPluginReceiveMessage(XPLMPluginID inFrom, int inMsg, void* inParam) {}

void draw_hello_world(XPLMWindowID inID, void* inRefcon) {
	XPLMSetGraphicsState(0, 0, 0, 0, 1, 1, 0);

	int l, t, r, b;
	XPLMGetWindowGeometry(inID, &l, &t, &r, &b);
	float col_white[] = { 1.0, 1.0, 1.0 };
	XPLMDrawString(col_white, l + 10, t - 20, const_cast<char*>("Hello, World!"), NULL, xplmFont_Proportional);
}