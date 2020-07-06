#include "SaveData.h"
#include "Charactor.h"
#include "DataBase.h"
#include "Application.h"
#include <algorithm>

using namespace std;

SaveData::SaveData()
{
	_mapNum = 0;
	Load();
}

SaveData::~SaveData()
{
}

bool SaveData::CreateSaveCharactorData(const std::vector<std::shared_ptr<Charactor>>& charactorVec)
{
	_charactorDataVec.clear();
	for (const auto& charactor : charactorVec)
	{
		if (charactor->GetTeam() != Team::player)continue;

		_charactorDataVec.emplace_back(CharactorData(charactor->GetCharactorType(), charactor->GetStartStatus()));
	}
	assert(_charactorDataVec.size() > 0);

	return true;
}

bool SaveData::CreateSaveData()
{
	// ���̃Z�[�u�f�[�^	������������csv�ɂ��邩��
	auto dataBase = Application::Instance().GetDataBase();
	_charactorDataVec.clear();
	_charactorDataVec.emplace_back(CharactorData(CharactorType::archer,		dataBase.GetLevelInitStatus(5, CharactorType::archer)));
	_charactorDataVec.emplace_back(CharactorData(CharactorType::priest,		dataBase.GetLevelInitStatus(5, CharactorType::priest)));
	_charactorDataVec.emplace_back(CharactorData(CharactorType::swordman,	dataBase.GetLevelInitStatus(5, CharactorType::swordman)));
	_charactorDataVec.emplace_back(CharactorData(CharactorType::warrior,	dataBase.GetLevelInitStatus(5, CharactorType::warrior)));
	_charactorDataVec.emplace_back(CharactorData(CharactorType::soldier,	dataBase.GetLevelInitStatus(5, CharactorType::soldier)));
	_charactorDataVec.emplace_back(CharactorData(CharactorType::mage,		dataBase.GetLevelInitStatus(5, CharactorType::mage)));

	SaveCharactorData(0);

	return true;
}

bool SaveData::SaveCharactorData(const CharactorData& charactorData, const unsigned int charactorDataIdx)
{
	if (charactorDataIdx < 0 || charactorDataIdx >= _charactorDataVec.size())
	{
		return false;
	}
	_charactorDataVec[charactorDataIdx] = charactorData;

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
	fwrite(_charactorDataVec.data(), sizeof(CharactorData) * cnt, 1, fp);

	// �N���A�}�b�v�̏�������
	_mapNum = max(_mapNum, mapNum);
	fwrite(&_mapNum, sizeof(_mapNum), 1, fp);

	fclose(fp);
}

bool SaveData::Load()
{
	_charactorDataVec.clear();
	FILE* fp = nullptr;

	fopen_s(&fp, "Resource/SaveData/savedata", "rb");

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
	fread_s(_charactorDataVec.data(), sizeof(CharactorData) * cnt, sizeof(CharactorData), cnt, fp);

	// �N���A�}�b�v�̓ǂݍ���
	fread_s(&_mapNum, sizeof(_mapNum), sizeof(_mapNum), 1, fp);

	fclose(fp);

	return true;
}

int SaveData::GetMapNum() const
{
	return _mapNum;
}

const std::vector<CharactorData>& SaveData::GetCharactorDataVec()const
{
	return _charactorDataVec;
}

CharactorData& SaveData::GetCharactorData(const unsigned int charactorData)
{
	return _charactorDataVec[charactorData];
}
