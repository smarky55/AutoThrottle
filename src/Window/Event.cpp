// This Source Code Form is subject to the terms of the Mozilla Public
// License, v.2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include <AutoThrottle/Window/Event.h>

ClickEvent::ClickEvent(Point point, MouseButton button, MouseAction action)
	: m_point(point), m_button(button), m_action(action)
{
}

ClickEvent::ClickEvent(Point point, MouseButton button, XPLMMouseStatus xpMouseStatus)
	: m_point(point), m_button(button)
{
	switch (xpMouseStatus) {
	case 1: // xplm_MouseDown
		m_action = MouseAction::Down;
		break;
	case 2: // xplm_MouseDrag
		m_action = MouseAction::Drag;
		break;
	case 3: // xplm_MouseUp
		m_action = MouseAction::Up;
		break;
	default:
		m_action = MouseAction::Down;
		break;
	}
}

const std::string& ClickEvent::type()
{
	static std::string s_type = "MouseEvent";
	return s_type;
}

const std::string& ClickEvent::eventType() const
{
	return type();
}

KeyEvent::KeyEvent(char key, XPLMKeyFlags flags)
	: m_key(key), m_flags(flags)
{
}

const std::string& KeyEvent::type()
{
	static std::string s_type = "KeyEvent";
	return s_type;
}

const std::string& KeyEvent::eventType() const
{
	return type();
}

WheelEvent::WheelEvent(Point point, int delta, bool horizontal /* = false */)
	: m_point(point), m_delta(delta), m_horizontal(horizontal)
{
}

const std::string& WheelEvent::type()
{
	static std::string s_type = "WheelEvent";
	return s_type;
}

const std::string& WheelEvent::eventType() const
{
	return type();
}
