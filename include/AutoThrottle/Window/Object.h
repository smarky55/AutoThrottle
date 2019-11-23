// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include <AutoThrottle/Util/geometry.h>

#include <memory>
#include <vector>

class Window;
class ClickEvent;
class KeyEvent;
class WheelEvent;

class Object;
using ObjectPtr = std::unique_ptr<Object>;

class Object {
public:
	Object();
	Object(Object* parent);
	virtual ~Object();

	void setParent(Object* parent);
	Object* parent();

	// Mouse click event, passes the event on to child objects
	// Returns true if the event has been consumed by this or any child
	bool mouseClick(const ClickEvent& event);

	// Mouse scroll event, passes the event on to child objects
	// Returns true if the event has been consumed by this or any child
	bool mouseWheel(const WheelEvent& event);

	// Key press event, passes the event on to child objects
	// Returns true if the event has been consumed by this or any child
	bool keyPress(const KeyEvent& event);

	void draw(Point anchor);

	// The rect that contains this object and its children, in local coordinates.
	Rect rect();

	// Compute the layout for this object and its children
	void layout();

	// Mark the layout of the current object and it's parent as needing recalculating
	void dirtyLayout();

	void setPadding(const Rect& padding);
	Rect padding();	

	void setBorder(const Rect& border);
	Rect border();	

	void setMargin(const Rect& margin);
	Rect margin();


protected:

	// Event handlers for overriding, return true to consume an event
	virtual bool onMouseClick(const ClickEvent& event) { return false; }
	virtual bool onMouseWheel(const WheelEvent& event) { return false; }
	virtual bool onKeyPress(const KeyEvent& event) { return false; }

	virtual void onDraw(Point anchor) {}

	// Compute the layout of this object and its children.
	// Subclasses may override this to assign themselves a fixed size or control
	// layout of their children.
	virtual void onLayout();

	Object* m_parent;

	Rect m_rect;
	bool m_layoutDirty;

	Rect m_margin;
	Rect m_border;
	Rect m_padding;
	std::vector<ObjectPtr> m_children;
};

