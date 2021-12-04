// This Source Code Form is subject to the terms of the Mozilla Public
// License, v.2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include <AutoThrottle/Window/Util/FontUtil.h>

#define STB_TRUETYPE_IMPLEMENTATION
#include <stb_truetype.h>

#include <filesystem>
#include <fstream>

Font::Font(const std::string& fontPath)
{
	std::ifstream fontFile(fontPath, std::ios::binary | std::ios::ate);
	if (!fontFile) {
		throw std::ios::failure("Failed to load file" + fontPath);
	}
	size_t fileLength = fontFile.tellg();
	fontFile.seekg(0);

	m_fontBuffer.resize(fileLength);
	fontFile.read(reinterpret_cast<char*>(m_fontBuffer.data()), fileLength);

	if (!stbtt_InitFont(&m_info, m_fontBuffer.data(), 0)) {
		throw std::runtime_error("Failed to load font info from " + fontPath);
	}
}

void Font::fillBitmap(std::vector<unsigned char>& buffer, const std::string& string, int height)
{
	std::vector<int> glyphSequence = getGlyphSequence(string);
	float scale = stbtt_ScaleForPixelHeight(&m_info, height);

	size_t requiredWidth = 0;
	for (size_t i = 0; i < glyphSequence.size(); ++i) {
		int advanceWidth, leftSidebearing;
		stbtt_GetGlyphHMetrics(&m_info, glyphSequence[i], &advanceWidth, &leftSidebearing);
		requiredWidth += advanceWidth;
		if (i < glyphSequence.size() - 1)
			requiredWidth += stbtt_GetGlyphKernAdvance(&m_info, glyphSequence[i], glyphSequence[i + 1]);
	}
	requiredWidth *= scale;

	if (buffer.capacity() < (requiredWidth * height)) {
		buffer.resize(requiredWidth * height);
	}

	int ascent, descent, lineGap;
	stbtt_GetFontVMetrics(&m_info, &ascent, &descent, &lineGap);

	ascent *= scale;
	descent *= scale;

	int x = 0;
	for (size_t i = 0; i < glyphSequence.size(); ++i) {
		int gx0, gx1, gy0, gy1;
		stbtt_GetGlyphBitmapBox(&m_info, glyphSequence[i], scale, scale, &gx0, &gy0, &gx1, &gy1);

		int y = ascent + gy0;

		int byteOffset = x + (y * requiredWidth);
		stbtt_MakeGlyphBitmap(&m_info, buffer.data() + byteOffset, gx1 - gx0, gy1 - gy0, requiredWidth, scale, scale, glyphSequence[i]);

		int ax;
		stbtt_GetGlyphHMetrics(&m_info, glyphSequence[i], &ax, 0);
		x += ax * scale;

		/* add kerning */
		int kern = 0;
		if (i < glyphSequence.size() - 1)
			kern = stbtt_GetGlyphKernAdvance(&m_info, glyphSequence[i], glyphSequence[i + 1]);
		x += kern * scale;
	}
}

std::vector<int> Font::getGlyphSequence(const std::string& string)
{
	std::vector<int> glyphSequence;
	for (int character : string) {
		int glyph;
		if (m_glyphs.find(character) != m_glyphs.end()) {
			glyph = m_glyphs[character];
		} else {
			glyph = stbtt_FindGlyphIndex(&m_info, character);
			m_glyphs[character] = glyph;
		}
		glyphSequence.push_back(glyph);
	}
	return glyphSequence;
}

FontManager* FontManager::get()
{
	static FontManager s_manager;
	return &s_manager;
}

void FontManager::createFont(const std::string& fontId, const std::string& fontPath)
{
	if (m_fonts.find(fontId) != m_fonts.end()) {
		return;
	}
	m_fonts[fontId] = std::make_unique<Font>(fontPath);
}

Font* FontManager::getFont(const std::string& fontId)
{
	if (m_fonts.find(fontId) == m_fonts.end()) {
		return nullptr;
	}
	return m_fonts[fontId].get();
}