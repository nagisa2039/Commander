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
	std::vector<CharactorData> _charactorDataForCreateSaveData;	// �Z�[�u�f�[�^�����p
	std::vector<CharactorData> _charactorDataVec;		// �L�����N�^�[�f�[�^(���ׂẴL�����N�^�[���Ǘ�)
	std::vector<CharactorData> _waitCharactorDataVec;	// �ҋ@���̃L�����N�^�[�f�[�^(�o�������L�����N�^�[�͂��Ȃ�)

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

	// ���݊J�����Ă���}�b�v�ԍ���Ԃ�	(�� : 5�Ȃ�0����4�܂ŃN���A���A5���J�����Ă���)
	int GetMapNum()const;

	// ��������Ԃ�
	unsigned int GetMoney();
	// ��������ݒ肷��
	void SetMoney(const unsigned int money);

	const std::vector<CharactorData>& GetCharactorDataVec()const;
	void SetWaitCharactorDataVec(const std::vector<CharactorData>& charactorDataVec);

	CharactorData& GetCharactorData(const unsigned int charactorData);
};

