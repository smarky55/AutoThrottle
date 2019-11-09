// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include <map>

using XPWidgetID = void*;

class Widget;

class WidgetRegistry {
	WidgetRegistry() = default;
	~WidgetRegistry() = default;
public:
	static WidgetRegistry* get();

	static void addWidget(XPWidgetID id, Widget* widget);
	static void removeWidget(XPWidgetID id);

	static Widget* getWidget(XPWidgetID id);

private:
	void m_add(XPWidgetID id, Widget* widget);
	void m_remove(XPWidgetID id);
	Widget* m_get(XPWidgetID id);

	std::map<XPWidgetID, Widget*> m_widgets;
};

