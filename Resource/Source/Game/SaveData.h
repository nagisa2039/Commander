#pragma once
#include <vector>
#include <memory>
#include "SaveDataCharactor.h"

class Charactor;

class SaveData
{
private:
	std::vector<SaveDataCharactor> _charactorDataVec;		// キャラクターデータ(すべてのキャラクターを管理)
	std::vector<SaveDataCharactor> _waitCharactorDataVec;	// 待機中のキャラクターデータ(出撃したキャラクターはいない)

	int _mapNum;
	unsigned int _money;

	bool CreateSaveCharactorData(const std::vector<std::shared_ptr<Charactor>>& charactorVec);
	bool CreateSaveData();
	void SaveCharactorData(const int& mapNum);

public:
	SaveData();
	~SaveData();

	bool Save(const std::vector<std::shared_ptr<Charactor>>& charactorVec, const int mapNum);
	bool Load();

	// 現在開放しているマップ番号を返す	(例 : 5なら0から4までクリアし、5を開放している)
	int GetMapNum()const;

	// 所持金を返す
	const unsigned int& GetMoney()const;
	// 所持金を設定する
	void SetMoney(const unsigned int money);

	const std::vector<SaveDataCharactor>& GetCharactorDataVec()const;
	void SetWaitCharactorDataVec(const std::vector<SaveDataCharactor>& charactorDataVec);

	SaveDataCharactor& GetCharactorData(const unsigned int charactorData);
};

