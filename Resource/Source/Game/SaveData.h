#pragma once
#include <vector>
#include <memory>
#include "SaveDataCharactor.h"

class Charactor;

class SaveData
{
private:
	/// <summary>
	/// 0未満なら未開放
	/// 0なら開放済み、未クリア
	/// 1以上ならクリアターン数
	/// </summary>
	std::vector<int> _clearMapDataVec;

	void WriteData();

public:
	SaveData();
	~SaveData();

	/// <summary>
	/// データのセーブ
	/// </summary>
	/// <param name="mapNum">クリアしたマップ番号</param>
	/// <param name="turnCnt">クリアに要したターン数</param>
	/// <returns>成否</returns>
	bool Save(const int mapNum, const int turnCnt);

	/// <summary>
	/// セーブデータのロード
	/// </summary>
	/// <returns>成否</returns>
	bool Load();

	/// <summary>
	/// セーブデータの初期化
	/// </summary>
	/// <returns>成否</returns>
	bool Reset();

	/// <summary>
	/// ステージの全開放
	/// </summary>
	/// <returns>成否</returns>
	bool AllOpen();

	/// <summary>
	/// マップ数分のクリア状況を取得
	/// 0未満なら未開放
	/// 0なら開放済
	/// 1以上ならクリアターン数
	/// </summary>
	/// <returns></returns>
	const std::vector<int>& GetClearMapDataVec()const;
};

