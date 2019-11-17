// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include <AutoThrottle/Window/CallbackWindow.h>

#include <XPLM/XPLMDisplay.h>

CallbackWindow::CallbackWindow(const Rect& rect, bool visible, XPLMWindowLayer layer, XPLMWindowDecoration decoration)
	: Window(rect, visible, layer, decoration)
{
}

CallbackWindow::CallbackWindow(const std::string& title, const Rect& rect, XPLMWindowLayer layer, XPLMWindowDecoration decoration, bool visible)
	: Window(title, rect, visible, layer, decoration)
{
}

CallbackWindow::~CallbackWindow()
{
}

void CallbackWindow::setDrawCallback(DrawCallback callback)
{
	m_drawCallback = callback;
}

void CallbackWindow::setKeyCallback(KeyCallback callback)
{
	m_keyCallback = callback;
}

void CallbackWindow::setLeftClickCallback(MouseClickCallback callback)
{
	m_leftClickCallback = callback;
}

void CallbackWindow::setRightClickCallback(MouseClickCallback callback)
{
	m_rightClickCallback = callback;
}

void CallbackWindow::setMouseWheelCallback(MouseWheelCallback callback)
{
	m_mouseWheelCallback = callback;
}

void CallbackWindow::setCursorCallback(CursorCallback callback)
{
	m_cursorCallback = callback;
}

void CallbackWindow::onDraw()
{
	if (m_drawCallback) {
		m_drawCallback();
	}
}

void CallbackWindow::onHandleKey(char key, XPLMKeyFlags flags, char virtualKey, int losingFocus)
{
	if (m_keyCallback) {
		m_keyCallback(key, flags, virtualKey, losingFocus);
	}
}

int CallbackWindow::onHandleLeftClick(int x, int y, XPLMMouseStatus status)
{
	if (m_leftClickCallback) {
		return m_leftClickCallback(x, y, status);
	}
	return 1;
}

int CallbackWindow::onHandleRightClick(int x, int y, XPLMMouseStatus status)
{
	if (m_rightClickCallback) {
		return m_rightClickCallback(x, y, status);
	}
	return 1;
}

int CallbackWindow::onHandleMouseWheel(int x, int y, int wheel, int delta)
{
	if (m_mouseWheelCallback) {
		return m_mouseWheelCallback(x, y, wheel, delta);
	}
	return 0;
}

XPLMCursorStatus CallbackWindow::onHandleCursor(int x, int y)
{
	if (m_cursorCallback) {
		return m_cursorCallback(x, y);
	}
	return xplm_CursorDefault;
}
