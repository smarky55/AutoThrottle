// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include <AutoThrottle/Window/Window.h>
#include <AutoThrottle/util.h>

#include <functional>
#include <string>

class CallbackWindow : public Window {
public:
	using DrawCallback = std::function<void()>;
	using KeyCallback = std::function<void(char, XPLMKeyFlags, char, int)>;
	using MouseClickCallback = std::function<int(int, int, XPLMMouseStatus)>;
	using MouseWheelCallback = std::function<int(int, int, int, int)>;
	using CursorCallback = std::function<XPLMCursorStatus(int, int)>;

	CallbackWindow(
		const Rect& rect,
		bool visible,
		XPLMWindowLayer layer,
		XPLMWindowDecoration decoration = 0 /*xplm_WindowDecorationNone*/
	);
	CallbackWindow(
		const std::string& title,
		const Rect& rect,
		XPLMWindowLayer layer,
		XPLMWindowDecoration decoration,
		bool visible
	);
	virtual ~CallbackWindow();

	void setDrawCallback(DrawCallback callback);
	void setKeyCallback(KeyCallback callback);
	void setLeftClickCallback(MouseClickCallback callback);
	void setRightClickCallback(MouseClickCallback callback);
	void setMouseWheelCallback(MouseWheelCallback callback);
	void setCursorCallback(CursorCallback callback);

protected:

	// Inherited from Window
	virtual void onDraw() override;
	virtual void onHandleKey(char key, XPLMKeyFlags flags, char virtualKey, int losingFocus) override;
	virtual int onHandleLeftClick(int x, int y, XPLMMouseStatus status) override;
	virtual int onHandleRightClick(int x, int y, XPLMMouseStatus status) override;
	virtual int onHandleMouseWheel(int x, int y, int wheel, int delta) override;
	virtual XPLMCursorStatus onHandleCursor(int x, int y) override;

private:
	DrawCallback m_drawCallback;
	KeyCallback m_keyCallback;
	MouseClickCallback m_leftClickCallback;
	MouseClickCallback m_rightClickCallback;
	MouseWheelCallback m_mouseWheelCallback;
	CursorCallback m_cursorCallback;
};