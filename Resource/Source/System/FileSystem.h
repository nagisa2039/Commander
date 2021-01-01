#pragma once
#include <memory>
#include<map>
#include <string>
#include <windows.h>

class SoundLoader;
class FontLoader;
class ImageLoader;

struct Size;

/// <summary>
/// リソースファイル管理クラス
/// </summary>
class FileSystem
{
private:
	FileSystem();
	FileSystem(const FileSystem&) = delete;
	FileSystem& operator=(const FileSystem&) = delete;

	// 画像読込クラス
	std::shared_ptr<ImageLoader> _imageLoader;
	// フォント読込クラス
	std::shared_ptr<FontLoader> _fontLoader;
	// 音読込クラス
	std::shared_ptr<SoundLoader> _soundLoder;

	/// <summary>
	/// フォントのセットアップ
	/// </summary>
	void FontSetUp();

public:
	~FileSystem();

	/// <summary>
	/// シングルトン実体を返す
	/// </summary>
	static FileSystem& Instance()
	{
		static FileSystem instance;
		return instance;
	}

	/// <summary>
	/// 画像ハンドルの取得
	/// </summary>
	/// <param name="path">画像ファイルパス</param>
	int GetImageHandle(const char* path);

	/// <summary>
	/// スクリーンを生成しハンドルを返す
	/// </summary>
	/// <param name="name">スクリーン名</param>
	/// <param name="screenSize">スクリーンサイズ</param>
	/// <param name="alpha">透明度を使うか</param>
	int MakeScreen(const char* name, const Size& screenSize, const bool alpha = false);

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
	/// <param name="fontUseName">フォント名</param>
	int GetFontHandle(std::string fontUseName);

	/// <summary>
	/// 音ハンドルの取得
	/// </summary>
	/// <param name="path">音ファイルパス</param>
	int GetSoundHandle(const char* path);

	/// <summary>
	/// 画像読込クラスの取得
	/// </summary>
	ImageLoader& GetImageLoader();

	/// <summary>
	/// 音ハンドルの取得
	/// </summary>
	SoundLoader& GetSoundLoader();

	/// <summary>
	/// フォントハンドルの取得
	/// </summary>
	FontLoader& GetFontLoader();

	/// <summary>
	/// ファイルパスからフォルダパスを抽出する
	/// </summary>
	/// <param name="">ファイルパス</param>
	static std::string GetFolderPass(std::string path);
};

// 画像ハンドル取得
#define ImageHandle(X) FileSystem::Instance().GetImageHandle(X)
// 音ハンドル取得
#define SoundHandle(X) FileSystem::Instance().GetSoundHandle(X)
// フォントハンドル取得
#define FontHandle(X)  FileSystem::Instance().GetFontHandle(X)

// 画像読込クラス取得
#define ImageL	FileSystem::Instance().GetImageLoader()
// 音読込クラス取得
#define SoundL	FileSystem::Instance().GetSoundLoader()
// フォント読込クラス取得
#define FontL	FileSystem::Instance().GetFontLoader()