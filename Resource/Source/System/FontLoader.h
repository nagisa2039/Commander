#pragma once
#include <map>
#include <string>
#include <windows.h>

/// <summary>
/// フォント読込クラス
/// </summary>
class FontLoader
{
private:
	// ﾌｫﾝﾄ登録用map
	std::map<LPCTSTR, int>		fontMap;	
	// 名前付けmap
	std::map<std::string, int> fontNameMap;

public:
	FontLoader();
	~FontLoader();

	/// <summary>
	/// フォントの初期化
	/// </summary>
	/// <param name="fontFile">フォントファイルパス</param>
	/// <param name="fontName">フォント名</param>
	/// <param name="useName">マップに登録し使用する際のフォント名</param>
	/// <param name="fontSize">フォントサイズ</param>
	/// <param name="fontThick">厚さ</param>
	/// <param name="edgeFlag">縁をつけるか</param>
	/// <param name="italic">イタリック体にするか</param>
	/// <returns>成否</returns>
	bool FontInit(LPCTSTR fontFile, LPCTSTR fontName, std::string useName, int fontSize, int fontThick, bool edgeFlag, bool italic);

	/// <summary>
	/// フォントハンドルの取得
	/// </summary>
	/// <param name="useName">フォント名</param>
	int GetFont(std::string useName);
};