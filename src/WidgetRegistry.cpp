// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include <AutoThrottle/WidgetRegistry.h>

WidgetRegistry* WidgetRegistry::get()
{
	static WidgetRegistry s_widgetRegistry;
	return &s_widgetRegistry;
}

void WidgetRegistry::addWidget(XPWidgetID id, Widget* widget)
{
	get()->m_add(id, widget);
}

void WidgetRegistry::removeWidget(XPWidgetID id)
{
	get()->m_remove(id);
}

Widget* WidgetRegistry::getWidget(XPWidgetID id)
{
	return get()->m_get(id);
}

void WidgetRegistry::m_add(XPWidgetID id, Widget* widget)
{
	m_widgets[id] = widget;
}

void WidgetRegistry::m_remove(XPWidgetID id)
{
	m_widgets.erase(id);
}

Widget* WidgetRegistry::m_get(XPWidgetID id)
{
	return m_widgets[id];
}
