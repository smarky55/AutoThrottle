// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include <AutoThrottle/Util/geometry.h>

#include <functional>
#include <map>
#include <memory>
#include <string>

using XPWidgetID = void*;
using XPWidgetPropertyID = int;
using XPWidgetMessage = int;


class Widget {
public:

	using Callback = std::function<int(XPWidgetMessage, XPWidgetID, intptr_t, intptr_t)>;

	Widget(const std::string& descriptor, 
		const Rect& rect,
		bool visible, 
		int widgetClass, 
		Widget* parent = nullptr);
	~Widget();

	Widget* createChild(const std::string& id, 
		const std::string& descriptor,
		const Rect& rect,
		bool visible,
		int widgetClass);

	Widget* getChild(const std::string& id);

	Widget* getParent();

	void setProperty(XPWidgetPropertyID property, intptr_t value);
	intptr_t getProperty(XPWidgetPropertyID property, int* exists = NULL);

	void setWidgetCallback(Callback callback);

	XPWidgetID id() const;

	void isVisible(bool visible);
	bool isVisible();
	void toggleVisible();

private:
	static int widgetCallback(XPWidgetMessage message, XPWidgetID xpWidget, intptr_t param1, intptr_t param2);

	XPWidgetID m_id;

	Rect m_rect;
	std::string m_descriptor;
	Widget* m_parent;

	std::map<std::string, std::unique_ptr<Widget>> m_children;
	Callback m_callback;
};

