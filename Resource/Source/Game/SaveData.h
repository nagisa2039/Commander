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
	std::vector<CharactorData> _startCharactorData;
	std::vector<CharactorData> _charactorDataVec;
	int _mapNum;

	bool CreateSaveCharactorData(const std::vector<std::shared_ptr<Charactor>>& charactorVec);
	bool CreateSaveData();
	void SaveCharactorData(const int& mapNum);
	void LoadStartPlayerCharactorData();

public:
	SaveData();
	~SaveData();

	bool SaveCharactorData(const CharactorData& charactorData, const unsigned int charactorDataIdx);
	bool Save(const std::vector<std::shared_ptr<Charactor>>& charactorVec, const int mapNum);
	bool Load();

	// 現在開放しているマップ番号を返す	(例 : 5なら0から4までクリアし、5を開放している)
	int GetMapNum()const;

	const std::vector<CharactorData>& GetCharactorDataVec()const;

	CharactorData& GetCharactorData(const unsigned int charactorData);
};

