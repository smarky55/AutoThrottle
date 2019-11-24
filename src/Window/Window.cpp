// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include <AutoThrottle/Window/Window.h>

#include <AutoThrottle/Window/Object.h>
#include <AutoThrottle/Window/Event.h>

#include <XPLM/XPLMDisplay.h>

Window::Window(const Rect& rect, bool visible, XPLMWindowLayer layer, XPLMWindowDecoration decoration)
	: m_rect(rect)
{
	XPLMCreateWindow_t params;
	params.structSize = sizeof(params);
	params.visible = visible;
	params.refcon = this;
	params.layer = layer;
	params.decorateAsFloatingWindow = decoration;

	params.left = m_rect.left;
	params.top = m_rect.top;
	params.right = m_rect.right;
	params.bottom = m_rect.bottom;

	params.drawWindowFunc = draw;
	params.handleKeyFunc = handleKey;
	params.handleMouseClickFunc = handleLeftClick;
	params.handleRightClickFunc = handleRightClick;
	params.handleMouseWheelFunc = handleMousewheel;
	params.handleCursorFunc = handleCursor;

	m_windowID = XPLMCreateWindowEx(&params);
}

Window::Window(const std::string& title, const Rect& rect, XPLMWindowLayer layer, XPLMWindowDecoration decoration, bool visible)
	: m_rect(rect)
{
	XPLMCreateWindow_t params;
	params.structSize = sizeof(params);
	params.visible = visible;
	params.refcon = this;
	params.layer = layer;
	params.decorateAsFloatingWindow = decoration;

	params.left = m_rect.left;
	params.top = m_rect.top;
	params.right = m_rect.right;
	params.bottom = m_rect.bottom;

	params.drawWindowFunc = draw;
	params.handleKeyFunc = handleKey;
	params.handleMouseClickFunc = handleLeftClick;
	params.handleRightClickFunc = handleRightClick;
	params.handleMouseWheelFunc = handleMousewheel;
	params.handleCursorFunc = handleCursor;

	m_windowID = XPLMCreateWindowEx(&params);
	XPLMSetWindowTitle(m_windowID, title.c_str());
}

Window::~Window()
{
	XPLMDestroyWindow(m_windowID);
}

void Window::setPositioningMode(XPLMWindowPositioningMode mode, int monitorIndex /* = -1 */)
{
	XPLMSetWindowPositioningMode(m_windowID, mode, monitorIndex);
}

void Window::setResizingLimits(int minWidth, int minHeight, int maxWidth, int maxHeight)
{
	XPLMSetWindowResizingLimits(m_windowID, minWidth, minHeight, maxWidth, maxHeight);
}

Rect Window::getGeometry()
{
	if (XPLMWindowIsPoppedOut(m_windowID)) {
		XPLMGetWindowGeometryOS(m_windowID, &m_rect.left, &m_rect.top, &m_rect.right, &m_rect.bottom);
	} else {
		XPLMGetWindowGeometry(m_windowID, &m_rect.left, &m_rect.top, &m_rect.right, &m_rect.bottom);
	}
	return m_rect;
}

void Window::setGeometry(Rect rect)
{
	m_rect = rect;
	if (XPLMWindowIsPoppedOut(m_windowID)) {
		XPLMSetWindowGeometryOS(m_windowID, m_rect.left, m_rect.top, m_rect.right, m_rect.bottom);
	} else {
		XPLMSetWindowGeometry(m_windowID, m_rect.left, m_rect.top, m_rect.right, m_rect.bottom);
	}
}

bool Window::isVisible() const
{
	return XPLMGetWindowIsVisible(m_windowID);
}

void Window::isVisible(bool visible)
{
	XPLMSetWindowIsVisible(m_windowID, visible);
}

void Window::setTitle(const std::string& title)
{
	XPLMSetWindowTitle(m_windowID, title.c_str());
}

void Window::takeKeyboardFocus()
{
	XPLMTakeKeyboardFocus(m_windowID);
}

bool Window::hasKeyboardFocus() const
{
	return XPLMHasKeyboardFocus(m_windowID);
}

void Window::bringToFront()
{
	XPLMBringWindowToFront(m_windowID);
}

bool Window::isInFront() const
{
	return XPLMIsWindowInFront(m_windowID);
}

void Window::addChild(std::unique_ptr<Object> child)
{
	m_rootObject->addChild(std::move(child));
}

void Window::draw(XPLMWindowID xpWindow, void* refcon)
{
	auto window = static_cast<Window*>(refcon);
	window->onDraw();
}

void Window::handleKey(XPLMWindowID xpWindow, char key, XPLMKeyFlags flags, char virtualKey, void* refcon, int losingFocus)
{
	auto window = static_cast<Window*>(refcon);
	window->onHandleKey(key, flags, virtualKey, losingFocus);
}

int Window::handleLeftClick(XPLMWindowID xpWindow, int x, int y, XPLMMouseStatus status, void* refcon)
{
	auto window = static_cast<Window*>(refcon);
	return window->onHandleLeftClick(x, y, status);
}

int Window::handleRightClick(XPLMWindowID xpWindow, int x, int y, XPLMMouseStatus status, void* refcon)
{
	auto window = static_cast<Window*>(refcon);
	return window->onHandleRightClick(x, y, status);
}

int Window::handleMousewheel(XPLMWindowID xpWindow, int x, int y, int wheel, int delta, void* refcon)
{
	auto window = static_cast<Window*>(refcon);
	return window->onHandleMouseWheel(x, y, wheel, delta);
}

XPLMCursorStatus Window::handleCursor(XPLMWindowID xpWindow, int x, int y, void* refcon)
{
	auto window = static_cast<Window*>(refcon);
	return window->onHandleCursor(x, y);
}

void Window::onDraw()
{
	// m_rect = m_rootObject->rect();
	// TODO: Resize window to contents?
	m_rootObject->draw({ m_rect.left, m_rect.bottom });
}

void Window::onHandleKey(char key, XPLMKeyFlags flags, char virtualKey, int losingFocus)
{
	KeyEvent event(key, flags);
	m_rootObject->keyPress(event);
}

int Window::onHandleLeftClick(int x, int y, XPLMMouseStatus status)
{
	ClickEvent event({ x, y }, MouseButton::Left, status);
	m_rootObject->mouseClick(event);
	return 1;
}

int Window::onHandleRightClick(int x, int y, XPLMMouseStatus status)
{
	ClickEvent event({ x, y }, MouseButton::Right, status);
	m_rootObject->mouseClick(event);
	return 1;
}

int Window::onHandleMouseWheel(int x, int y, int wheel, int delta)
{
	WheelEvent event({ x, y }, delta, wheel);
	m_rootObject->mouseWheel(event);
	return 1;
}

XPLMCursorStatus Window::onHandleCursor(int x, int y)
{
	return xplm_CursorDefault;
}
