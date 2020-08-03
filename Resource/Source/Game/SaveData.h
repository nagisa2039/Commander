#pragma once
#include <vector>
#include <memory>
#include "SaveDataCharactor.h"

class Charactor;

class SaveData
{
private:
	std::vector<SaveDataCharactor> _charactorDataVec;		// �L�����N�^�[�f�[�^(���ׂẴL�����N�^�[���Ǘ�)
	std::vector<SaveDataCharactor> _waitCharactorDataVec;	// �ҋ@���̃L�����N�^�[�f�[�^(�o�������L�����N�^�[�͂��Ȃ�)

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

	// ���݊J�����Ă���}�b�v�ԍ���Ԃ�	(�� : 5�Ȃ�0����4�܂ŃN���A���A5���J�����Ă���)
	int GetMapNum()const;

	// ��������Ԃ�
	const unsigned int& GetMoney()const;
	// ��������ݒ肷��
	void SetMoney(const unsigned int money);

	const std::vector<SaveDataCharactor>& GetCharactorDataVec()const;
	void SetWaitCharactorDataVec(const std::vector<SaveDataCharactor>& charactorDataVec);

	SaveDataCharactor& GetCharactorData(const unsigned int charactorData);
};

