#pragma once
#include <vector>
#include "CharactorType.h"
#include "Status.h"
#include <memory>

class Charactor;

struct CharactorData
{
	CharactorType charType;
	Status status;
	CharactorData() :charType(CharactorType::max), status(Status()) {};
	CharactorData(const CharactorType ct, const Status& st) :charType(ct), status(st) {};
};

class SaveData
{
private:
	std::vector<CharactorData> _charactorDataForCreateSaveData;	// セーブデータ生成用
	std::vector<CharactorData> _charactorDataVec;		// キャラクターデータ(すべてのキャラクターを管理)
	std::vector<CharactorData> _waitCharactorDataVec;	// 待機中のキャラクターデータ(出撃したキャラクターはいない)

	int _mapNum;
	unsigned int _money;

	bool CreateSaveCharactorData(const std::vector<std::shared_ptr<Charactor>>& charactorVec);
	bool CreateSaveData();
	void SaveCharactorData(const int& mapNum);
	void LoadStartPlayerCharactorData();

public:
	SaveData();
	~SaveData();

	bool Save(const std::vector<std::shared_ptr<Charactor>>& charactorVec, const int mapNum);
	bool Load();

	// 現在開放しているマップ番号を返す	(例 : 5なら0から4までクリアし、5を開放している)
	int GetMapNum()const;

	// 所持金を返す
	unsigned int GetMoney();
	// 所持金を設定する
	void SetMoney(const unsigned int money);

	const std::vector<CharactorData>& GetCharactorDataVec()const;
	void SetWaitCharactorDataVec(const std::vector<CharactorData>& charactorDataVec);

	CharactorData& GetCharactorData(const unsigned int charactorData);
};

