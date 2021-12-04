// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include <AutoThrottle/Window/Window.h>

#include <AutoThrottle/Window/Object.h>
#include <AutoThrottle/Window/Event.h>

#include <XPLM/XPLMDisplay.h>

Window::Window(const Rect& rect, bool visible, XPLMWindowLayer layer, XPLMWindowDecoration decoration)
  : Window("", rect, layer, decoration, visible) {}

Window::Window(
  const std::string& title,
  const Rect& rect,
  bool visible,
  XPLMWindowLayer layer,
  XPLMWindowDecoration decoration
) : m_rect(rect) {
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

  params.drawWindowFunc = drawWindow;
  params.handleKeyFunc = handleKey;
  params.handleMouseClickFunc = handleLeftClick;
  params.handleRightClickFunc = handleRightClick;
  params.handleMouseWheelFunc = handleMousewheel;
  params.handleCursorFunc = handleCursor;

  m_windowID = XPLMCreateWindowEx(&params);
  XPLMSetWindowTitle(m_windowID, title.c_str());

  m_rootObject = std::make_unique<Object>();
}

Window::~Window() {
  XPLMDestroyWindow(m_windowID);
}

void Window::setPositioningMode(XPLMWindowPositioningMode mode, int monitorIndex /* = -1 */) {
  XPLMSetWindowPositioningMode(m_windowID, mode, monitorIndex);
}

void Window::setResizingLimits(int minWidth, int minHeight, int maxWidth, int maxHeight) {
  XPLMSetWindowResizingLimits(m_windowID, minWidth, minHeight, maxWidth, maxHeight);
}

Rect Window::getGeometry() {
  if (XPLMWindowIsPoppedOut(m_windowID)) {
    XPLMGetWindowGeometryOS(m_windowID, &m_rect.left, &m_rect.top, &m_rect.right, &m_rect.bottom);
  } else {
    XPLMGetWindowGeometry(m_windowID, &m_rect.left, &m_rect.top, &m_rect.right, &m_rect.bottom);
  }
  return m_rect;
}

void Window::setGeometry(Rect rect) {
  m_rect = rect;
  if (XPLMWindowIsPoppedOut(m_windowID)) {
    XPLMSetWindowGeometryOS(m_windowID, m_rect.left, m_rect.top, m_rect.right, m_rect.bottom);
  } else {
    XPLMSetWindowGeometry(m_windowID, m_rect.left, m_rect.top, m_rect.right, m_rect.bottom);
  }
}

bool Window::isVisible() const {
  return XPLMGetWindowIsVisible(m_windowID);
}

void Window::isVisible(bool visible) {
  XPLMSetWindowIsVisible(m_windowID, visible);
}

void Window::setTitle(const std::string& title) {
  XPLMSetWindowTitle(m_windowID, title.c_str());
}

void Window::takeKeyboardFocus() {
  XPLMTakeKeyboardFocus(m_windowID);
}

bool Window::hasKeyboardFocus() const {
  return XPLMHasKeyboardFocus(m_windowID);
}

void Window::bringToFront() {
  XPLMBringWindowToFront(m_windowID);
}

bool Window::isInFront() const {
  return XPLMIsWindowInFront(m_windowID);
}

void Window::addChild(std::unique_ptr<Object> child) {
  m_rootObject->addChild(std::move(child));
}

void Window::drawWindow(XPLMWindowID xpWindow, void* refcon) {
  auto window = static_cast<Window*>(refcon);
  window->draw();
}

void Window::handleKey(XPLMWindowID xpWindow, char key, XPLMKeyFlags flags, char virtualKey, void* refcon, int losingFocus) {
  auto window = static_cast<Window*>(refcon);
  window->keyPress(key, flags, virtualKey, losingFocus);
}

int Window::handleLeftClick(XPLMWindowID xpWindow, int x, int y, XPLMMouseStatus status, void* refcon) {
  auto window = static_cast<Window*>(refcon);
  return window->leftClick(x, y, status);
}

int Window::handleRightClick(XPLMWindowID xpWindow, int x, int y, XPLMMouseStatus status, void* refcon) {
  auto window = static_cast<Window*>(refcon);
  return window->rightClick(x, y, status);
}

int Window::handleMousewheel(XPLMWindowID xpWindow, int x, int y, int wheel, int delta, void* refcon) {
  auto window = static_cast<Window*>(refcon);
  return window->scroll(x, y, wheel, delta);
}

XPLMCursorStatus Window::handleCursor(XPLMWindowID xpWindow, int x, int y, void* refcon) {
  auto window = static_cast<Window*>(refcon);
  return window->onHandleCursor(x, y);
}

void Window::draw() {
  // TODO: Resize window to contents?
  Rect windowGeometry = getGeometry();

  // Call virtual draw
  onDraw();

  // Draw children
  m_rootObject->layout();
  m_rootObject->draw({windowGeometry.left, windowGeometry.bottom});
}

void Window::keyPress(char key, XPLMKeyFlags flags, char virtualKey, int losingFocus) {
  KeyEvent event(key, flags);

  // Try to handle the key with the window
  if (!onHandleKey(event)) {
    // Pass keypress to root object
    m_rootObject->keyPress(event);
  }
}

bool Window::leftClick(int x, int y, XPLMMouseStatus status) {
  ClickEvent event({x, y}, MouseButton::Left, status);

  if (!onClick(event)) {
    m_rootObject->mouseClick(event);
  }
  return true;
}

bool Window::rightClick(int x, int y, XPLMMouseStatus status) {
  ClickEvent event({x, y}, MouseButton::Right, status);

  if (!onClick(event)) {
    m_rootObject->mouseClick(event);
  }
  return true;
}

bool Window::scroll(int x, int y, int wheel, int delta) {
  WheelEvent event({x, y}, delta, wheel);
  
  if (!onScroll(event)) {
    m_rootObject->mouseWheel(event);
  }
  return true;
}

void Window::onDraw() {
  // Do nothing
}

bool Window::onHandleKey(const KeyEvent& keyEvent) {
  return false;

}

bool Window::onClick(const ClickEvent& clickEvent) {
  return false;
}

bool Window::onScroll(const WheelEvent& wheelEvent) {
  return false;
}

XPLMCursorStatus Window::onHandleCursor(int x, int y) {
  return xplm_CursorDefault;
}
