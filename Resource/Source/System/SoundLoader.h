#pragma once
#include <unordered_map>
#include <string.h>

/// <summary>
/// 音読込クラス
/// </summary>
class SoundLoader
{
private:
	// リソース管理配列
	std::unordered_map<std::string, int> _table;

public:
	SoundLoader()=default;
	~SoundLoader()=default;

	/// <summary>
	/// 音ハンドルの取得
	/// </summary>
	/// <param name="path">音ファイルパス</param>
	int GetSoundHandle(const char* path);

	/// <summary>
	/// BGMの再生
	/// </summary>
	/// <param name="path">ファイルパス</param>
	/// <param name="volume">音量</param>
	/// <param name="playTop">先頭から再生</param>
	bool PlayBGM(const char* path, const int volume = 255, const bool playTop = true);

	/// <summary>
	/// BGMの再生
	/// </summary>
	/// <param name="handle">音ハンドル</param>
	/// <param name="volume">音量</param>
	/// <param name="playTop">先頭から再生</param>
	bool PlayBGM(const int handle, const int volume = 255, const bool playTop = true);

	/// <summary>
	/// SEの再生
	/// </summary>
	/// <param name="path">ファイルパス</param>
	/// <param name="volume">音量</param>
	/// <returns></returns>
	bool PlaySE(const char* path, const int volume = 255);

	/// <summary>
	/// SEの再生
	/// </summary>
	/// <param name="handle">音ハンドル</param>
	/// <param name="volume">音量</param>
	/// <returns></returns>
	bool PlaySE(const int handle, const int volume = 255);

	/// <summary>
	/// 音を停止
	/// </summary>
	/// <param name="handle">音ハンドル</param>
	bool StopSound(const int handle);

	/// <summary>
	/// 全ての音を停止
	/// </summary>
	bool StopAllSound();
};

