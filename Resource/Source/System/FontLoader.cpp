#include "FontLoader.h"
#include <DxLib.h>
#include <cassert>

FontLoader::FontLoader()
{
}

FontLoader::~FontLoader()
{
}

bool FontLoader::FontInit(LPCTSTR fontFile, LPCTSTR fontName, std::string useName, int fontSize, int fontThick, bool edgeFlag, bool italic)
{
	if (fontMap.find(fontFile) == fontMap.end())
	{
		// �t�H���g�t�@�C���̒ǉ�
		fontMap[fontFile] = AddFontResourceEx(fontFile, FR_PRIVATE, NULL);
	}

	if (fontNameMap.find(useName) == fontNameMap.end())
	{
		if (edgeFlag)
		{
			// �����ر��������
			fontNameMap[useName] = CreateFontToHandle(fontName, fontSize, fontThick, DX_FONTTYPE_ANTIALIASING_EDGE_8X8, -1, 2, italic);
		}
		else
		{
			// �����ر�����Ȃ�
			fontNameMap[useName] = CreateFontToHandle(fontName, fontSize, fontThick, DX_FONTTYPE_ANTIALIASING_8X8, -1, -1, italic);
		}
		return fontNameMap.at(useName) != -1;

	}
	return true;
}

int FontLoader::GetFont(std::string useName)
{
	assert(fontNameMap.find(useName) != fontNameMap.end());

	return fontNameMap[useName];
}