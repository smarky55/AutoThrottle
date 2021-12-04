// This Source Code Form is subject to the terms of the Mozilla Public
// License, v.2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include "Object.h"

#include <string>
#include <vector>

class Label : public Object {
public:
	Label(const std::string& text, Object* parent = nullptr);

	void setText(const std::string& text);
	const std::string& text() const;

	void setTextHeight(int textHeight);
	int textHeight() const;

protected:
	virtual void onDraw(Point anchor) override;

	virtual void onLayout() override;

	void genTexture();

private:
	std::string m_text;
	int m_textHeight;
	int m_textWidth;

	int m_textureId = -1;
	size_t m_textureWidth = 1;
	size_t m_textureHeight = 1;
	std::vector<unsigned char> m_image;
};

