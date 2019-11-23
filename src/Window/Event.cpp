#include <AutoThrottle/Window/Event.h>

ClickEvent::ClickEvent(Point point, MouseButton button)
	: m_point(point), m_button(button)
{
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
