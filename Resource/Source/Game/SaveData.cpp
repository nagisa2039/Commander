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

	constexpr int LOCK = -1;	// ������Ă��Ȃ�
	constexpr int UNLOCK = 0;	// �N���A���Ă��Ȃ�
	constexpr char FILE_NAME[] = "savedata";	// �Z�[�u�f�[�^�̃t�@�C����
	constexpr char FOLDER_NAME[] = "Resource/SaveData/";	// �Z�[�u�f�[�^�̃t�H���_

	constexpr double VERSION = 1.0;
}

SaveData::SaveData()
{
	Load();
}

SaveData::~SaveData()
{
}

bool SaveData::Save(const int mapNum, const int turnCnt)
{
	// �V�K�쐬�Ȃ�
	if (mapNum < 0)
	{
		std::fill(_clearMapDataVec.begin(), _clearMapDataVec.end(), LOCK);
		_clearMapDataVec[0] = UNLOCK;
	}
	else
	{
		auto mapCnt = _clearMapDataVec.size();
		assert(mapNum < mapCnt);
		assert(turnCnt > 0);
		if (_clearMapDataVec[mapNum] <= 0)
		{
			_clearMapDataVec[mapNum] = turnCnt;
		}
		else
		{
			_clearMapDataVec[mapNum] = min(turnCnt, _clearMapDataVec[mapNum]);
		}
		if (mapNum + 1 < mapCnt)
		{
			_clearMapDataVec[static_cast<size_t>(mapNum) + 1] = UNLOCK;
		}
	}

	WriteData();

	return true;
}

void SaveData::WriteData()
{
	FILE* fp = nullptr;
	stringstream ss;
	ss << FOLDER_NAME << FILE_NAME;
	fopen_s(&fp, ss.str().c_str(), "wb");

	assert(fp != nullptr);

	// �o�[�W�����̏�������
	fwrite(&VERSION, sizeof(VERSION), 1, fp);

	// �}�b�v���̏�������
	int mapCnt = static_cast<int>(_clearMapDataVec.size());
	assert(mapCnt > 0);
	fwrite(&mapCnt, sizeof(mapCnt), 1, fp);

	// �}�b�v�̃N���A�󋵂���������
	auto eleSize = sizeof(_clearMapDataVec[0]);
	fwrite(_clearMapDataVec.data(), eleSize, _clearMapDataVec.size(), fp);

	// 8�o�C�g��؂�ɂ��邽�߂̋l�ߕ�
	std::vector<uint8_t> stuffings((_clearMapDataVec.size() + 1) * eleSize % 8);
	fwrite(stuffings.data(), sizeof(uint8_t), stuffings.size(), fp);

	fclose(fp);
}

bool SaveData::Load()
{
	auto& mapDatas = DataBase::Instance().GetMapDataTable();
	_clearMapDataVec.resize(mapDatas.size());

	FILE* fp = nullptr;

	// �f�B���N�g���̑��݊m�F
	struct stat statBuf;
	if (stat(FOLDER_NAME, &statBuf) != 0)
	{
		// ���������̂ō쐬
		int result = _mkdir(FOLDER_NAME);
		assert(result == 0);
	}
	stringstream ss;
	ss << FOLDER_NAME << FILE_NAME;
	fopen_s(&fp, ss.str().c_str(), "rb");

	if (fp == NULL)
	{
		Reset();
		return false;
	}

	// �Z�[�u�f�[�^�̃o�[�W�����Ǎ��̓ǂݍ���
	double version;
	fread_s(&version, sizeof(version), sizeof(version), 1, fp);
	if (version != VERSION)
	{
		Reset();
		return false;
	}
	
	// �N���A�}�b�v�̓ǂݍ���
	int mapNum;
	fread_s(&mapNum, sizeof(mapNum), sizeof(mapNum), 1, fp);

	// �Z�[�u�f�[�^�̃}�b�v���ƃf�[�^�x�[�X�̃}�b�v�����Ⴄ�Ȃ�Z�[�u�f�[�^����蒼��
	if (mapNum != mapDatas.size())
	{
		// �Z�[�u�f�[�^��V�K�쐬
		Reset();
		return false;
	}

	int eleSize = sizeof(_clearMapDataVec[0]);
	auto eleCnt = _clearMapDataVec.size();
	fread_s(_clearMapDataVec.data(), eleSize* eleCnt,eleSize, eleCnt, fp);

	fclose(fp);

	return true;
}

bool SaveData::Reset()
{
	return Save(-1, -1);
}

bool SaveData::AllOpen()
{
	for (auto& clearMapData : _clearMapDataVec)
	{
		clearMapData = max(clearMapData, 0);
	}

	WriteData();
	return true;
}

const std::vector<int>& SaveData::GetClearMapDataVec() const
{
	return _clearMapDataVec;
}
