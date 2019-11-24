// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include <AutoThrottle/Util/geometry.h>

#include <string>

class Event {
public:
	virtual const std::string& eventType() const = 0;

};

// -------------------------------------------------------------
// ClickEvent

enum class MouseButton {
	Left,
	Right
};

enum class MouseAction {
	Down,
	Drag,
	Up
};

using XPLMMouseStatus = int;

class ClickEvent : public Event {
public:
	ClickEvent(Point point, MouseButton button, MouseAction action);
	ClickEvent(Point point, MouseButton button, XPLMMouseStatus xpMouseStatus);

	static const std::string& type();
	virtual const std::string& eventType() const override;

	Point point() const { return m_point; }
	MouseButton button() const { return m_button; }
	MouseAction action() const { return m_action; }

private:
	Point m_point;
	MouseButton m_button;
	MouseAction m_action;
};

// -------------------------------------------------------------
// KeyEvent

using XPLMKeyFlags = int;

class KeyEvent : public Event {
public:
	KeyEvent(char key, XPLMKeyFlags flags);

	static const std::string& type();
	virtual const std::string& eventType() const override;

	char key() const { return m_key; }
	XPLMKeyFlags flags() const { return m_flags; }

private:
	char m_key;
	XPLMKeyFlags m_flags;
};

// -------------------------------------------------------------
// WheelEvent

class WheelEvent : public Event {
public:
	WheelEvent(Point point, int delta, bool horizontal = false);

	static const std::string& type();
	virtual const std::string& eventType() const override;

	Point point() const { return m_point; }
	int delta() const { return m_delta; }
	bool horizontal() const { return m_horizontal; }

private:
	Point m_point;
	int m_delta;
	bool m_horizontal;
};