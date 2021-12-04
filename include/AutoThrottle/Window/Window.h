// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include <AutoThrottle/Util/geometry.h>

#include <functional>
#include <memory>
#include <string>

using XPLMWindowID = void*;
using XPLMWindowLayer = int;
using XPLMWindowDecoration = int;
using XPLMKeyFlags = int;
using XPLMMouseStatus = int;
using XPLMCursorStatus = int;
using XPLMWindowPositioningMode = int;

class Object;
class KeyEvent;
class ClickEvent;
class WheelEvent;

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
    bool visible,
    XPLMWindowLayer layer,
    XPLMWindowDecoration decoration
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

  void addChild(std::unique_ptr<Object> child);

protected:
  // Draw window geometry.
  virtual void onDraw();

  // Handle keypress. Return true to indicate key event has been handled.
  virtual bool onHandleKey(const KeyEvent& keyEvent);

  // Handle click. Return true to indicate click has been handled.
  virtual bool onClick(const ClickEvent& clickEvent);

  // Handle mouse scroll. Return true to indicate click has been handled.
  virtual bool onScroll(const WheelEvent& wheelEvent);

  virtual XPLMCursorStatus onHandleCursor(int x, int y);

private:
  // X plane window callbacks
  static void drawWindow(XPLMWindowID xpWindow, void* refcon);
  static void handleKey(XPLMWindowID xpWindow, char key, XPLMKeyFlags flags, char virtualKey, void* refcon, int losingFocus);
  static int handleLeftClick(XPLMWindowID xpWindow, int x, int y, XPLMMouseStatus status, void* refcon);
  static int handleRightClick(XPLMWindowID xpWindow, int x, int y, XPLMMouseStatus status, void* refcon);
  static int handleMousewheel(XPLMWindowID xpWindow, int x, int y, int wheel, int delta, void* refcon);
  static XPLMCursorStatus handleCursor(XPLMWindowID xpWindow, int x, int y, void* refcon);

  // Draw window and all child objects
  void draw();

  // Called when window receives key press event
  void keyPress(char key, XPLMKeyFlags flags, char virtualKey, int losingFocus);

  bool leftClick(int x, int y, XPLMMouseStatus status);

  bool rightClick(int x, int y, XPLMMouseStatus status);

  bool scroll(int x, int y, int wheel, int delta);

  Rect m_rect;
  XPLMWindowID m_windowID;

  std::unique_ptr<Object> m_rootObject;
};