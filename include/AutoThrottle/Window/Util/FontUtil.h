// This Source Code Form is subject to the terms of the Mozilla Public
// License, v.2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#pragma once

#include <stb_truetype.h>

#include <map>
#include <memory>
#include <string>
#include <vector>

class Font;

class FontManager {
public:
	static FontManager* get();

	void createFont(const std::string& fontId, const std::string& fontPath);

	Font* getFont(const std::string& fontId);

private:
	std::map<std::string, std::unique_ptr<Font>> m_fonts;
};

class Font {
public:
	Font(const std::string& fontPath);

	void fillBitmap(std::vector<unsigned char>& buffer, const std::string& string, int height);

	std::vector<int> getGlyphSequence(const std::string& string);
private:
	std::vector<unsigned char> m_fontBuffer;
	std::map<int, int> m_glyphs;

	stbtt_fontinfo m_info;
};
