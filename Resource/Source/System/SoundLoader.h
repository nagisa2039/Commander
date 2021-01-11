#pragma once
#include <unordered_map>
#include <string.h>

/// <summary>
/// 音読込クラス
/// </summary>
class SoundLoader
{
private:
	// ハンドル管理マップ<ファイルパス, ハンドル>
	std::unordered_map<std::string, int> _handleTable;

	// ハンドル情報
	struct HandleInf
	{
		// ボリューム
		int volume = 255;
		// 再生状況
		bool play = false;
	};

	// 音量管理マップ<ハンドル, >
	std::unordered_map<int, HandleInf> _handleInfTable;
	// マスターボリューム
	float _masterVolume;

	/// <summary>
	/// 音量の変更
	/// </summary>
	/// <param name="handle">音ハンドル</param>
	/// <param name="volume">音量</param>
	void ChangeVolume(const int handle, const int volume);

public:
	SoundLoader();
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

	/// <summary>
	/// マスターボリュームの変更(0～255)
	/// </summary>
	void ChangeMasterVolume(const int volume);

	/// <summary>
	/// ハンドルの再生状況を取得
	/// </summary>
	/// <param name="handle">音ハンドル</param>
	/// <returns>再生してるか</returns>
	bool PlayCheck(const int handle);
};

