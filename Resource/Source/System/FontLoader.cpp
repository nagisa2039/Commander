#include "FontLoader.h"
#include <DxLib.h>

FontLoader::FontLoader()
{
}

FontLoader::~FontLoader()
{
}

bool FontLoader::Load(const char* path, Data& data)
{
	return false;
}

void FontLoader::Unload(const char* path)
{
}

bool FontLoader::FontInit(LPCTSTR fontFile, LPCTSTR fontName, std::string useName, int fontSize, int fontThick, bool edgeFlag, bool italic)
{
	if (fontMap.find(fontFile) == fontMap.end())
	{
		// フォントファイルの追加
		fontMap[fontFile] = AddFontResourceEx(fontFile, FR_PRIVATE, NULL);
	}

	if (fontNameMap.find(useName) == fontNameMap.end())
	{
		if (edgeFlag)
		{
			// ｱﾝﾁｴｲﾘｱｽ･縁あり
			fontNameMap[useName] = CreateFontToHandle(fontName, fontSize, fontThick, DX_FONTTYPE_ANTIALIASING_EDGE_8X8, -1, 2, italic);
		}
		else
		{
			// ｱﾝﾁｴｲﾘｱｽ･縁なし
			fontNameMap[useName] = CreateFontToHandle(fontName, fontSize, fontThick, DX_FONTTYPE_ANTIALIASING_8X8, -1, -1, italic);
		}
		return fontNameMap.at(useName) != -1;

	}
	return true;
}

int FontLoader::SetFont(std::string useName)
{
	return fontNameMap[useName];
}