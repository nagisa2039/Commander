#pragma once
#include "Loader.h"
#include <map>
#include <string>
#include <windows.h>

class FontLoader :
	public Loader
{
private:
	std::map<LPCTSTR, int>		fontMap;		// ﾌｫﾝﾄ登録用map
	std::map<std::string, int> fontNameMap;	// 名前付けmap

public:
	FontLoader();
	~FontLoader();

	bool Load(const char* path, Data& data)override final;
	void Unload(const char* path)override final;

	// ﾌｫﾝﾄの読み込み、参照で返す
	bool FontInit(LPCTSTR fontFile, LPCTSTR fontName, std::string useName, int fontSize, int fontThick, bool edgeFlag, bool italic);

	int SetFont(std::string useName);		// 音楽のｾｯﾄ(つけた名前)非ﾙｰﾌﾟ用
};