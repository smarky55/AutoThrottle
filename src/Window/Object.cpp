// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include <AutoThrottle/Window/Object.h>

#include <AutoThrottle/Window/Event.h>

Object::Object(Object* parent /* = nullptr */)
	: m_layoutDirty(true), m_parent(parent)
{
}

Object::~Object()
{
}

void Object::setParent(Object* parent)
{
	m_parent = parent;
}

Object* Object::parent()
{
	return m_parent;
}

void Object::addChild(ObjectPtr child)
{
	child->setParent(this);
	m_children.push_back(std::move(child));
}

bool Object::mouseClick(const ClickEvent& event)
{
	for (ObjectPtr& child : m_children) {
		if (!child->rect().contains(event.point()))
			continue;

		if (child->mouseClick(event)) {
			return true;
		}
	}
	return onMouseClick(event);
}

bool Object::mouseWheel(const WheelEvent& event)
{
	for (ObjectPtr& child : m_children) {
		if (!child->rect().contains(event.point()))
			continue;

		if (child->mouseWheel(event)) {
			return true;
		}
	}
	return onMouseWheel(event);
}

bool Object::keyPress(const KeyEvent& event)
{
	for (ObjectPtr& child : m_children) {

		if (child->keyPress(event)) {
			return true;
		}
	}
	return onKeyPress(event);
}



void Object::draw(Point anchor)
{
	onDraw(anchor);
	anchor += {m_margin.left, m_margin.bottom};
	anchor += {m_border.left, m_border.bottom};
	anchor += {m_padding.left, m_padding.bottom};

	for (ObjectPtr& child : m_children) {
		child->draw(anchor);
	}
}

Rect Object::rect()
{
	layout();
	return m_rect;
}

void Object::layout()
{
	if (m_layoutDirty) {
		for (ObjectPtr& child : m_children) {
			child->layout();
		}
		onLayout();
	}
	m_layoutDirty = false;
}

void Object::setPadding(const Rect& padding)
{
	m_padding = padding;
}

Rect Object::padding()
{
	return m_padding;
}

void Object::setBorder(const Rect& border)
{
	m_border = border;
}

Rect Object::border()
{
	return m_border;
}

void Object::setMargin(const Rect& margin)
{
	m_margin = margin;
}

Rect Object::margin()
{
	return m_margin;
}

void Object::dirtyLayout()
{
	m_layoutDirty = true;
	if (m_parent) {
		m_parent->dirtyLayout();
	}
}

void Object::onLayout()
{
	m_rect = Rect();
	for (ObjectPtr& child : m_children) {
		m_rect = minimumRect(m_rect, child->rect());
	}
	m_rect += m_padding + m_border + m_margin;
}
