// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include <AutoThrottle/Widget.h>
#include <AutoThrottle/WidgetRegistry.h>

#include <XPWidgets.h>

Widget::Widget(const std::string& descriptor, const Rect& rect, bool visible, int widgetClass, Widget* parent)
	: m_rect(rect),
	m_descriptor(descriptor),
	m_parent(parent)
{
	bool isRoot = true;
	XPWidgetID parentId = NULL;
	if (parent) {
		isRoot = false;
		parentId = parent->m_id;
	}
	m_id = XPCreateWidget(rect.left, rect.top, rect.right, rect.bottom, 
		visible, descriptor.c_str(), isRoot, parentId, widgetClass);

	WidgetRegistry::addWidget(m_id, this);
}

Widget::~Widget()
{
	// Let child widgets destroy themselves in their destructors
	XPDestroyWidget(m_id, false);
	WidgetRegistry::removeWidget(m_id);
}

Widget* Widget::createChild(const std::string& id, const std::string& descriptor, const Rect& rect, bool visible, int widgetClass)
{
	return (m_children[id] = std::make_unique<Widget>(descriptor, rect, visible, widgetClass, this)).get();
}

Widget* Widget::getChild(const std::string& id)
{
	if (m_children.count(id)) {
		return m_children.at(id).get();
	} 
	return nullptr;
}

Widget* Widget::getParent()
{
	return m_parent;
}

void Widget::setProperty(XPWidgetPropertyID property, intptr_t value)
{
	XPSetWidgetProperty(m_id, property, value);
}

intptr_t Widget::getProperty(XPWidgetPropertyID property, int* exists)
{
	return XPGetWidgetProperty(m_id, property, exists);
}

void Widget::setWidgetCallback(Callback callback)
{
	using namespace std::placeholders;
	m_callback = callback;
	XPAddWidgetCallback(m_id, widgetCallback);
}

XPWidgetID Widget::id() const
{
	return m_id;
}

void Widget::isVisible(bool visible)
{
	if (visible) {
		XPShowWidget(m_id);
	} else {
		XPHideWidget(m_id);
	}
}

bool Widget::isVisible()
{
	return XPIsWidgetVisible(m_id);
}

void Widget::toggleVisible()
{
	isVisible(!isVisible());
}

int Widget::widgetCallback(XPWidgetMessage message, XPWidgetID xpWidget, intptr_t param1, intptr_t param2)
{
	Widget* widget = WidgetRegistry::getWidget(xpWidget);
	return widget->m_callback(message, xpWidget, param1, param2);
}
