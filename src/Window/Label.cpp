// This Source Code Form is subject to the terms of the Mozilla Public
// License, v.2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include <AutoThrottle/Window/Label.h>

#include <XPLM/XPLMGraphics.h>

#include <cmath>

Label::Label(const std::string& text, Object* parent)
	: Object(parent), m_text(text)
{
}

void Label::setText(const std::string& text)
{
	m_text = text;
	dirtyLayout();
}

const std::string& Label::text()
{
	return m_text;
}

void Label::onDraw(Point anchor)
{
	Rect rect = m_rect + anchor; // rect in world space
	float colour[] = { 1.0f, 1.0f, 1.0f };
	XPLMDrawString(colour, rect.left, rect.bottom, (char*)m_text.c_str(), NULL, xplmFont_Proportional);
}

void Label::onLayout()
{
	int height;
	XPLMGetFontDimensions(xplmFont_Proportional, NULL, &height, NULL);

	int width = std::ceil(XPLMMeasureString(xplmFont_Proportional, m_text.c_str(), m_text.length()));

	m_rect = Rect(0, 0, width, height);
}
