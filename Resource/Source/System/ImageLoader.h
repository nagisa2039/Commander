#pragma once
#include <map>
#include <string>

struct Size;

/// <summary>
/// 画像読込クラス
/// </summary>
class ImageLoader
{
private:
	// リソース管理配列
	std::map<std::string, int> _table;
public:
	ImageLoader();
	~ImageLoader();

	/// <summary>
	/// 画像ハンドルの取得
	/// </summary>
	/// <param name="path">画像ハンドルの取得</param>
	int GetImageHandle(const char* path);

	/// <summary>
	/// マスク画像の読み込み
	/// </summary>
	/// <param name="path">ファイルパス</param>
	int LoadMask(const char* path);

	/// <summary>
	/// スクリーンの生成
	/// </summary>
	/// <param name="name">スクリーン名</param>
	/// <param name="screenSize">スクリーンサイズ</param>
	/// <param name="alpha">透明度を使うか</param>
	int MakeScreen(const char* name, const Size& screenSize, bool alpha = false);
};