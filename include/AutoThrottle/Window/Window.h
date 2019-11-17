// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include <AutoThrottle/util.h>

#include <functional>
#include <string>

using XPLMWindowID = void*;
using XPLMWindowLayer = int;
using XPLMWindowDecoration = int;
using XPLMKeyFlags = int;
using XPLMMouseStatus = int;
using XPLMCursorStatus = int;
using XPLMWindowPositioningMode = int;

class Window {
public:
	Window(
		const Rect& rect, 
		bool visible,
		XPLMWindowLayer layer, 
		XPLMWindowDecoration decoration = 0 /*xplm_WindowDecorationNone*/
	);
	Window(
		const std::string& title,
		const Rect& rect,
		XPLMWindowLayer layer,
		XPLMWindowDecoration decoration,
		bool visible
	);
	virtual ~Window();

	void setPositioningMode(XPLMWindowPositioningMode mode, int monitorIndex = -1);

	void setResizingLimits(int minWidth, int minHeight, int maxWidth, int maxHeight);

	Rect getGeometry();
	void setGeometry(Rect rect);

	bool isVisible() const;
	void isVisible(bool visible);

	void setTitle(const std::string& title);

	void takeKeyboardFocus();
	bool hasKeyboardFocus() const;

	void bringToFront();
	bool isInFront() const;

protected:
	static void draw(XPLMWindowID xpWindow, void* refcon);
	static void handleKey(XPLMWindowID xpWindow, char key, XPLMKeyFlags flags, char virtualKey, void* refcon, int losingFocus);
	static int handleLeftClick(XPLMWindowID xpWindow, int x, int y, XPLMMouseStatus status, void* refcon);
	static int handleRightClick(XPLMWindowID xpWindow, int x, int y, XPLMMouseStatus status, void* refcon);
	static int handleMousewheel(XPLMWindowID xpWindow, int x, int y, int wheel, int delta, void* refcon);
	static XPLMCursorStatus handleCursor(XPLMWindowID xpWindow, int x, int y, void* refcon);

	virtual void onDraw();
	virtual void onHandleKey(char key, XPLMKeyFlags flags, char virtualKey, int losingFocus);
	virtual int onHandleLeftClick(int x, int y, XPLMMouseStatus status);
	virtual int onHandleRightClick(int x, int y, XPLMMouseStatus status);
	virtual int onHandleMouseWheel(int x, int y, int wheel, int delta);
	virtual XPLMCursorStatus onHandleCursor(int x, int y);

protected:
	Rect m_rect;
	XPLMWindowID m_windowID;

};