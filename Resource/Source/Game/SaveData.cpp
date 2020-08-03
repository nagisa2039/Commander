#include <algorithm>
#include <fstream>
#include <sstream>
#include <direct.h>
#include <sys/stat.h>
#include "SaveData.h"
#include "Charactor.h"
#include "DataBase.h"
#include "Application.h"

using namespace std;

namespace
{
	constexpr unsigned int INIT_MONEY = 114514;
}

SaveData::SaveData()
{
	_mapNum = 0;

	_waitCharactorDataVec.clear();
	_charactorDataVec.clear();

	Load();
}

SaveData::~SaveData()
{
}

bool SaveData::CreateSaveCharactorData(const std::vector<std::shared_ptr<Charactor>>& charactorVec)
{
	_charactorDataVec = _waitCharactorDataVec;
	for (const auto& charactor : charactorVec)
	{
		if (charactor->GetTeam() != Team::player)continue;

		_charactorDataVec.emplace_back(SaveDataCharactor(charactor->GetCharactorType(), charactor->GetStartStatus()));
	}
	assert(_charactorDataVec.size() > 0);

	return true;
}

bool SaveData::CreateSaveData()
{
	_charactorDataVec.clear();
	_charactorDataVec = Application::Instance().GetDataBase().GetSaveDataCharactors();
	_waitCharactorDataVec = _charactorDataVec;
	_money = INIT_MONEY;
	SaveCharactorData(0);
	return true;
}

bool SaveData::Save(const std::vector<std::shared_ptr<Charactor>>& charactorVec, const int mapNum)
{
	CreateSaveCharactorData(charactorVec);

	SaveCharactorData(mapNum + 1);

	return true;
}

void SaveData::SaveCharactorData(const int& mapNum)
{
	FILE* fp = nullptr;
	fopen_s(&fp, "Resource/SaveData/savedata", "wb");

	// �L�����N�^�[���̏�������
	unsigned int cnt = static_cast<unsigned int>(_charactorDataVec.size());
	fwrite(&cnt, sizeof(cnt), 1, fp);
	// �L�����N�^�[�f�[�^�̏�������
	fwrite(_charactorDataVec.data(), sizeof(SaveDataCharactor) * cnt, 1, fp);

	// �N���A�}�b�v�̏�������
	_mapNum = max(_mapNum, mapNum);
	fwrite(&_mapNum, sizeof(_mapNum), 1, fp);

	// �������̏�������
	fwrite(&_money, sizeof(_money), 1, fp);

	fclose(fp);
}

bool SaveData::Load()
{
	_charactorDataVec.clear();
	FILE* fp = nullptr;

	// �f�B���N�g���̑��݊m�F
	const char* dir = "Resource/SaveData/";
	struct stat statBuf;
	if (stat(dir, &statBuf) != 0)
	{
		// ���������̂ō쐬
		assert(_mkdir(dir) == 0);
	}

	const char* fileName = "savedata";
	stringstream ss;
	ss << dir << fileName;
	fopen_s(&fp, ss.str().c_str(), "rb");

	if (fp == NULL)
	{
		CreateSaveData();
		return false;
	}

	// �L�����N�^�[�T�C�Y�̓ǂݍ���
	int cnt = 0;
	fread_s(&cnt, sizeof(cnt), sizeof(cnt), 1, fp);
	_charactorDataVec.resize(cnt);

	// �L�����N�^�[�f�[�^�̓ǂݍ���
	fread_s(_charactorDataVec.data(), sizeof(SaveDataCharactor) * cnt, sizeof(SaveDataCharactor), cnt, fp);
	_waitCharactorDataVec = _charactorDataVec;

	// �N���A�}�b�v�̓ǂݍ���
	fread_s(&_mapNum, sizeof(_mapNum), sizeof(_mapNum), 1, fp);

	// �������̓ǂݍ���
	fread_s(&_money, sizeof(_money), sizeof(_money), 1, fp);

	fclose(fp);

	return true;
}

int SaveData::GetMapNum() const
{
	return _mapNum;
}

const unsigned int& SaveData::GetMoney() const
{
	return _money;
}

void SaveData::SetMoney(const unsigned int money)
{
	_money = money;
}

const std::vector<SaveDataCharactor>& SaveData::GetCharactorDataVec()const
{
	return _charactorDataVec;
}

void SaveData::SetWaitCharactorDataVec(const std::vector<SaveDataCharactor>& charactorDataVec)
{
	_waitCharactorDataVec = charactorDataVec;
}

SaveDataCharactor& SaveData::GetCharactorData(const unsigned int charactorData)
{
	return _charactorDataVec[charactorData];
}
