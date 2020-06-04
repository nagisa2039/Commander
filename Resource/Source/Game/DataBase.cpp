#include "DataBase.h"
#include <fstream>
#include <sstream>
#include "Application.h"
#include "FileSystem.h"
#include "SaveData.h"

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

	// キャラクターデータテーブルの読み込み
	{
		ifstream ifs("Resource/DataBase/Charactor.csv");
		string line;
		vector<string> outputVec;
		outputVec.reserve(20);
		// 最初の行はスキップ
		getline(ifs, line);
		int idx = 0;
		while (getline(ifs, line))
		{
			split(line, ',', outputVec);
			CharactorData charactorData;
			// キャラクターID
			// 職業名
			charactorData.name = outputVec[1];
			// 攻撃範囲
			charactorData.range = Range(atoi(outputVec[2].c_str()), atoi(outputVec[3].c_str()));
			// 初期ステータス
			charactorData.initialStatus = Status(1, atoi(outputVec[4].c_str()), atoi(outputVec[5].c_str()), atoi(outputVec[6].c_str()),
				atoi(outputVec[7].c_str()), atoi(outputVec[8].c_str()), atoi(outputVec[9].c_str()), atoi(outputVec[10].c_str()), atoi(outputVec[17].c_str()), outputVec[18] != "", outputVec[19] != "");
			// ステータス成長率
			charactorData.statusGrowRate = Status(1, atoi(outputVec[11].c_str()), atoi(outputVec[12].c_str()), atoi(outputVec[13].c_str()),
				atoi(outputVec[14].c_str()), atoi(outputVec[15].c_str()), atoi(outputVec[16].c_str()), atoi(outputVec[10].c_str()), atoi(outputVec[17].c_str()), outputVec[18] != "", outputVec[19] != "");
			// キャラクター画像パス
			charactorData.ImagePath = outputVec[20];
			// 職業アイコンパス
			charactorData.iconImagePath = outputVec[21];

			_charactorDataTable[idx++] = charactorData;
		}
	}

	// マップチップテーブルの読み込み
	{
		ifstream ifs("Resource/DataBase/MapChip.csv");
		string line;
		vector<string> outputVec;
		outputVec.reserve(20);
		// 最初の行はスキップ
		getline(ifs, line);
		int idx = 0;
		while (getline(ifs, line))
		{
			split(line, ',', outputVec);
			if (outputVec[0] == "")break;

			MapChipData mapChipData;
			// 名前
			mapChipData.name = outputVec[1];
			// パス
			mapChipData.drawData.path = outputVec[2];
			// 画像の左上座標
			mapChipData.drawData.leftup = Vector2Int(atoi(outputVec[3].c_str()), atoi(outputVec[4].c_str()));
			// 画像のサイズ
			mapChipData.drawData.size = Size(atoi(outputVec[5].c_str()), atoi(outputVec[6].c_str()));
			// 簡略化色
			mapChipData.simpleColor = atoi(outputVec[7].c_str());
			// 移動コスト
			mapChipData.moveCost = atoi(outputVec[8].c_str());
			// 防御補正値
			mapChipData.defense = atoi(outputVec[9].c_str());
			// 回避補正値
			mapChipData.avoidance = atoi(outputVec[10].c_str());
			// 回復量
			mapChipData.recovery = atoi(outputVec[11].c_str());

			_mapChipDataTable[idx] = mapChipData;
			idx++;
		}
	}

	// 経験値テーブルの読み込み
	{
		_expDataTable.reserve(50);
		ifstream ifs("Resource/DataBase/Exp.csv");
		string line;
		vector<string> outputVec;
		outputVec.reserve(3);
		// 最初の行はスキップ
		getline(ifs, line);
		while (getline(ifs, line))
		{
			split(line, ',', outputVec);
			if (outputVec[0] == "")break;
			_expDataTable.emplace_back(ExpData(atoi(outputVec[1].c_str()), atoi(outputVec[2].c_str())));
		}
	}

	// 属性テーブルの読み込み
	{
		_attributeDataTable.reserve(10);
		ifstream ifs("Resource/DataBase/Attribute.csv");
		string line;
		vector<string> outputVec;
		outputVec.reserve(10);
		// 最初の行はスキップ
		getline(ifs, line);
		while (getline(ifs, line))
		{
			split(line, ',', outputVec);
			if (outputVec[0] == "")break;
			_attributeDataTable.emplace_back(AttributeData(outputVec[1], atoi(outputVec[2].c_str())));
		}
	}

	// rateTableの読み込み
	{
		ifstream ifs("Resource/DataBase/AttributeRate.csv");
		string line;
		vector<string> outputVec;
		outputVec.reserve(5);
		_attributeRateTable.resize(_attributeDataTable.size());
		int attributeIdx = 0;
		// 最初の行はスキップ
		getline(ifs, line);
		while (getline(ifs, line))
		{
			split(line, ',', outputVec);
			if (outputVec[0] == "")break;
			_attributeRateTable[attributeIdx].resize(_attributeDataTable.size());
			for (int idx = 0; idx < _attributeRateTable[attributeIdx].size(); idx++)
			{
				_attributeRateTable[attributeIdx][idx] = atof(outputVec[idx + 1].c_str());
			}
			attributeIdx++;
		}
	}

	// mapDataTableの読み込み
	{
		ifstream ifs("Resource/DataBase/Map.csv");
		string line;
		vector<string> outputVec;
		outputVec.reserve(5);
		// 最初の行はスキップ
		getline(ifs, line);
		while (getline(ifs, line))
		{
			split(line, ',', outputVec);
			if (outputVec[0] == "")break;
			_mapDataTable.emplace_back(MapData(outputVec[1], outputVec[2]));
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

Status DataBase::GetLevelInitStatus(const uint8_t level, const CharactorType charType)const
{
	Status status;
	auto charactorData = Application::Instance().GetDataBase().GetCharactorData(charType);
	status = charactorData.initialStatus;
	status.level = level;
	status.health += level * charactorData.statusGrowRate.health / 100.0f;
	status.power += level * charactorData.statusGrowRate.power / 100.0f;
	status.defense += level * charactorData.statusGrowRate.defense / 100.0f;
	status.magic_defense += level * charactorData.statusGrowRate.magic_defense / 100.0f;
	status.speed += level * charactorData.statusGrowRate.speed / 100.0f;
	status.skill += level * charactorData.statusGrowRate.skill / 100.0f;
	return status;
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

const std::vector<DataBase::MapData>& DataBase::GetMapDataTable() const
{
	return _mapDataTable;
}

const DataBase::MapData& DataBase::GetMapData(const unsigned int mapDataId) const
{
	return _mapDataTable.at(mapDataId);
}