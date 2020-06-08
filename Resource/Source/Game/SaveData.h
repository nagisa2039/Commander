#pragma once
#include <vector>
#include "CharactorType.h"
#include "Status.h"
#include <memory>

class Charactor;

class SaveData
{
private:
	struct CharactorData
	{
		CharactorType charType;
		Status status;
		CharactorData() :charType(CharactorType::max), status(Status()) {};
		CharactorData(const CharactorType ct, const Status& st) :charType(ct), status(st) {};
	};
	std::vector<CharactorData> _charactorDataVec;
	int _mapNum;

	bool CreateSaveCharactorData(const std::vector<std::shared_ptr<Charactor>>& charactorVec);
	bool CreateSaveData();
	void SaveCharactorData(const int& mapNum);

public:
	SaveData();
	~SaveData();

	bool Save(const std::vector<std::shared_ptr<Charactor>>& charactorVec, const int mapNum);
	bool Load();

	// ���݊J�����Ă���}�b�v�ԍ���Ԃ�	(�� : 5�Ȃ�0����4�܂ŃN���A���A5���J�����Ă���)
	int GetMapNum()const;

	const std::vector<CharactorData>& GetCharactorDataVec()const;

};
