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

SaveData::SaveData()
{
	_mapNum = 0;

	LoadStartPlayerCharactorData();

	Load();
}

void SaveData::LoadStartPlayerCharactorData()
{
	auto split = [](const string& input, const char delimiter, vector<string>& output)
	{
		istringstream stream(input);
		string field;
		output.clear();
		while (getline(stream, field, delimiter))
		{
			output.emplace_back(field);
		}
	};

	// �L�����N�^�[�f�[�^�e�[�u���̓ǂݍ���
	{
		_startCharactorData.reserve(6); 
		ifstream ifs("Resource/DataBase/StartPlayerCharactor.csv");
		string line;
		vector<string> outputVec;
		outputVec.reserve(4);
		// �ŏ��̍s�̓X�L�b�v
		getline(ifs, line);
		int idx = 0;
		auto& dataBase = Application::Instance().GetDataBase();
		while (getline(ifs, line))
		{
			split(line, ',', outputVec);
			auto charactorType = static_cast<CharactorType>(atoi(outputVec[1].c_str()));
			auto status = dataBase.GetLevelInitStatus(atoi(outputVec[2].c_str()), charactorType);
			status.weaponId = atoi(outputVec[3].c_str());
			_startCharactorData.emplace_back(charactorType, status);
		}
	}
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
	_charactorDataVec.clear();
	_charactorDataVec = _startCharactorData;
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

	// �f�B���N�g���̑��݊m�F
	const char* dir = "Resource/SaveData";
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
