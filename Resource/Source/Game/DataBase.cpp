#include "DataBase.h"
#include <fstream>
#include <sstream>
#include "Application.h"
#include "FileSystem.h"

using namespace std;

DataBase::DataBase()
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
		ifstream ifs("Resource/DataBase/charactorDataBase.csv");
		string line;
		vector<string> outputVec;
		outputVec.reserve(20);
		// �ŏ��̍s�̓X�L�b�v
		getline(ifs, line);
		int idx = 0;
		while (getline(ifs, line))
		{
			split(line, ',', outputVec);
			CharactorData charactorData;
			// �L�����N�^�[ID
			// �E�Ɩ�
			charactorData.name = outputVec[1];
			// �U���͈�
			charactorData.range = Range(atoi(outputVec[2].c_str()), atoi(outputVec[3].c_str()));
			// �����X�e�[�^�X
			charactorData.initialStatus = Status(1, atoi(outputVec[4].c_str()), atoi(outputVec[5].c_str()), atoi(outputVec[6].c_str()),
				atoi(outputVec[7].c_str()), atoi(outputVec[8].c_str()), atoi(outputVec[9].c_str()), atoi(outputVec[10].c_str()), atoi(outputVec[17].c_str()), outputVec[18] != "", outputVec[19] != "");
			// �X�e�[�^�X������
			charactorData.statusGrowRate = Status(1, atoi(outputVec[11].c_str()), atoi(outputVec[12].c_str()), atoi(outputVec[13].c_str()),
				atoi(outputVec[14].c_str()), atoi(outputVec[15].c_str()), atoi(outputVec[16].c_str()), atoi(outputVec[10].c_str()), atoi(outputVec[17].c_str()), outputVec[18] != "", outputVec[19] != "");
			// �L�����N�^�[�摜�p�X
			charactorData.ImagePath = outputVec[20];
			// �E�ƃA�C�R���p�X
			charactorData.iconImagePath = outputVec[21];

			_charactorDataTable[idx++] = charactorData;
		}
	}

	// �}�b�v�`�b�v�e�[�u���̓ǂݍ���
	{
		ifstream ifs("Resource/DataBase/mapChipDataBase.csv");
		string line;
		vector<string> outputVec;
		outputVec.reserve(20);
		// �ŏ��̍s�̓X�L�b�v
		getline(ifs, line);
		int idx = 0;
		while (getline(ifs, line))
		{
			split(line, ',', outputVec);
			MapChipData mapChipData;
			// ���O
			mapChipData.name = outputVec[1];
			// �p�X
			mapChipData.drawData.path = outputVec[2];
			// �摜�̍�����W
			mapChipData.drawData.leftup = Vector2Int(atoi(outputVec[3].c_str()), atoi(outputVec[4].c_str()));
			// �摜�̃T�C�Y
			mapChipData.drawData.size = Size(atoi(outputVec[5].c_str()), atoi(outputVec[6].c_str()));
			// �ȗ����F
			mapChipData.simpleColor = atoi(outputVec[7].c_str());
			// �ړ��R�X�g
			mapChipData.moveCost = atoi(outputVec[8].c_str());
			// �h��␳�l
			mapChipData.defense = atoi(outputVec[9].c_str());
			// ���␳�l
			mapChipData.avoidance = atoi(outputVec[10].c_str());
			// �񕜗�
			mapChipData.recovery = atoi(outputVec[11].c_str());

			_mapChipDataTable[idx] = mapChipData;
			idx++;
		}
	}

	// �o���l�e�[�u���̓ǂݍ���
	{
		_expDataTable.reserve(50);
		ifstream ifs("Resource/DataBase/expDataBase.csv");
		string line;
		vector<string> outputVec;
		outputVec.reserve(3);
		// �ŏ��̍s�̓X�L�b�v
		getline(ifs, line);
		while (getline(ifs, line))
		{
			split(line, ',', outputVec);
			_expDataTable.emplace_back(ExpData(atoi(outputVec[1].c_str()), atoi(outputVec[2].c_str())));
		}
	}

	// �����e�[�u���̓ǂݍ���
	{
		_attributeDataTable.reserve(10);
		ifstream ifs("Resource/DataBase/attributeDataBase.csv");
		string line;
		vector<string> outputVec;
		outputVec.reserve(10);
		// �ŏ��̍s�̓X�L�b�v
		getline(ifs, line);
		while (getline(ifs, line))
		{
			split(line, ',', outputVec);
			if (outputVec.size() <= 0)break;
			_attributeDataTable.emplace_back(AttributeData(outputVec[1], atoi(outputVec[2].c_str())));
		}
	}

	// rateTable�̓ǂݍ���
	{
		ifstream ifs("Resource/DataBase/attributeRateDataBase.csv");
		string line;
		vector<string> outputVec;
		outputVec.reserve(5);
		_attributeRateTable.resize(_attributeDataTable.size());
		int attributeIdx = 0;
		// �ŏ��̍s�̓X�L�b�v
		getline(ifs, line);
		while (getline(ifs, line))
		{
			split(line, ',', outputVec);
			_attributeRateTable[attributeIdx].resize(_attributeDataTable.size());
			for (int idx = 0; idx < _attributeRateTable[attributeIdx].size(); idx++)
			{
				_attributeRateTable[attributeIdx][idx] = atof(outputVec[idx + 1].c_str());
			}
			attributeIdx++;
		}
	}
}

DataBase::~DataBase()
{
}

float DataBase::GetAttributeRate(const unsigned int selfAtributeId, const unsigned int targetAtributeId)const
{
	if (_attributeRateTable.size() <= selfAtributeId
		|| _attributeRateTable[selfAtributeId].size() <= targetAtributeId)
	{
		return 0.0f;
	}

	return _attributeRateTable.at(selfAtributeId).at(targetAtributeId);
}

int DataBase::GetCharactorImageHandle(const CharactorType charactorType, const Team team) const
{
	char str[256];
	sprintf_s(str, 256, "%s%s", GetCharactorData(charactorType).ImagePath.c_str(), team == Team::player ? "_player.png" : "_enemy.png");
	return Application::Instance().GetFileSystem()->GetImageHandle(str);
}

const DataBase::CharactorData& DataBase::GetCharactorData(const CharactorType charactorType) const
{
	return _charactorDataTable.at(static_cast<size_t>(charactorType));
}

const DataBase::MapChipData& DataBase::GetMapChipData(const Map_Chip mapChip) const
{
	return _mapChipDataTable.at(static_cast<size_t>(mapChip));
}

const DataBase::ExpData& DataBase::GetExpData(const uint8_t level) const
{
	return _expDataTable.at(level);
}

const DataBase::AttributeData& DataBase::GetAttributeData(const unsigned int attributeId)const
{
	return _attributeDataTable.at(attributeId);
}
