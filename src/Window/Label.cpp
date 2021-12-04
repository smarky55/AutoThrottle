// This Source Code Form is subject to the terms of the Mozilla Public
// License, v.2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include <AutoThrottle/Window/Label.h>
#include <AutoThrottle/Window/Util/FontUtil.h>

#include <XPLM/XPLMDisplay.h>
#include <XPLM/XPLMGraphics.h>
#include <XPLM/XPLMUtilities.h>

#include <GL/glew.h>
#if LIN
#include <GL/gl.h>
#elif __GNUC__
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

#include <cmath>

Label::Label(const std::string& text, Object* parent)
	: Object(parent), m_text(text), m_textHeight(20), m_textWidth(0)
{
	XPLMGenerateTextureNumbers(&m_textureId, 1);
	char sysPath[512];
	XPLMGetSystemPath(sysPath);
	FontManager::get()->createFont("Roboto-Regular", std::string(sysPath) + "Resources\\fonts\\Roboto-Regular.ttf");

	glewInit();
}

void Label::setText(const std::string& text)
{
	m_text = text;
	dirtyLayout();
}

const std::string& Label::text() const
{
	return m_text;
}

void Label::setTextHeight(int textHeight)
{
	m_textHeight = textHeight;
	dirtyLayout();
}

int Label::textHeight() const
{
	return m_textHeight;
}

void Label::onDraw(Point anchor)
{
	Rect rect = m_rect + anchor; // rect in world space
	float colour[] = { 1.0f, 1.0f, 1.0f };
	
	XPLMSetGraphicsState(0, 1, 0, 0, 1, 0, 0);

	XPLMBindTexture2d(m_textureId, 0);

	float texX = static_cast<float>(m_textWidth) / m_textureWidth;
	float texY = 1 - static_cast<float>(m_textHeight) / m_textureHeight;
	
	glColor3f(1, 1, 1);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 1); glVertex2f(rect.left, rect.top);
	glTexCoord2f(0, texY); glVertex2f(rect.left, rect.bottom);
	glTexCoord2f(texX, texY); glVertex2f(rect.right, rect.bottom);
	glTexCoord2f(texX, 1); glVertex2f(rect.right, rect.top);
	glEnd();
}

void Label::onLayout()
{
	genTexture();

	m_rect = Rect(0, 0, m_textWidth, m_textHeight);
}

void Label::genTexture()
{
	XPLMDebugString("[AutoThrottle] Info: Generating label texture");
	std::vector<unsigned char> strBuffer;
	Font* font = FontManager::get()->getFont("Roboto-Regular");
	font->fillBitmap(strBuffer, m_text, m_textHeight);
	m_textWidth = strBuffer.size() / m_textHeight;

	size_t textureWidth = 1;
	while (textureWidth < m_textWidth) {
		if (textureWidth == 2048) {
			XPLMDebugString("[AutoThrottle] Warning: Creating a label texture wider than 2k!\n");
		}
		textureWidth = textureWidth << 1;
	}

	size_t textureHeight = 1;
	while (textureHeight < m_textHeight) {
		if (textureHeight == 2048) {
			XPLMDebugString("[AutoThrottle] Warning: Creating a label texture taller than 2k!\n");
		}
		textureHeight = textureHeight << 1;
	}

	// Resize texture to minimum requred
	bool neededResizing = false;
	if (textureWidth != m_textureWidth || textureHeight != m_textureHeight) {
		neededResizing = true;
		m_textureWidth = textureWidth;
		m_textureHeight = textureHeight;
		m_image.resize(textureWidth * textureHeight);		
	}

	// Fill with zeros
	std::fill(m_image.begin(), m_image.end(), 0);

	for (size_t y = 0; y < m_textHeight; ++y) {
		// Copy strbuffer into image
		memcpy(m_image.data() + m_textureWidth * y, strBuffer.data() + m_textWidth * y, m_textWidth);
	}

	XPLMBindTexture2d(m_textureId, 0);

	if (GLEW_ARB_texture_swizzle) {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_G, GL_RED);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_B, GL_RED);
	} else if (GLEW_EXT_texture_swizzle) {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_G_EXT, GL_RED);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_B_EXT, GL_RED);
	}

	if (neededResizing) {
		glTexImage2D(
			GL_TEXTURE_2D,
			0, 
			GL_RED, 
			m_textureWidth, 
			m_textureHeight,
			0, 
			GL_RED, 
			GL_UNSIGNED_BYTE,
			m_image.data()
		);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	} else {
		glTexSubImage2D(
			GL_TEXTURE_2D,
			0,
			0,
			0,
			m_textureWidth,
			m_textureHeight,
			GL_RED,
			GL_UNSIGNED_BYTE,
			m_image.data()
		);
	}
}
