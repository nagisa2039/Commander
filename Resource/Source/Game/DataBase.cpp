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

			// 武器種ID
			charactorData.weaponType = atoi(outputVec[2].c_str());

			// 初期ステータス
			int initSbeginIdx = 3;
			charactorData.initialStatus = 
				Status(1, atoi(outputVec[initSbeginIdx].c_str()), atoi(outputVec[initSbeginIdx+1].c_str()), atoi(outputVec[initSbeginIdx+2].c_str()),
					atoi(outputVec[initSbeginIdx+3].c_str()), atoi(outputVec[initSbeginIdx+4].c_str()), atoi(outputVec[initSbeginIdx+5].c_str()), 
					atoi(outputVec[initSbeginIdx+6].c_str()), atoi(outputVec[initSbeginIdx+7].c_str()), atoi(outputVec[initSbeginIdx+8].c_str()));
		
			// ステータス成長率
			int growSbeginIdx = 12;
			charactorData.statusGrowRate =
				Status(1, atoi(outputVec[growSbeginIdx].c_str()), atoi(outputVec[growSbeginIdx + 1].c_str()), atoi(outputVec[growSbeginIdx + 2].c_str()),
					atoi(outputVec[growSbeginIdx + 3].c_str()), atoi(outputVec[growSbeginIdx + 4].c_str()), atoi(outputVec[growSbeginIdx + 5].c_str()), 
					atoi(outputVec[growSbeginIdx + 6].c_str()), atoi(outputVec[growSbeginIdx + 7].c_str()), 0);

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

	//// 経験値テーブルの読み込み
	//{
	//	_expDataTable.reserve(50);
	//	ifstream ifs("Resource/DataBase/Exp.csv");
	//	string line;
	//	vector<string> outputVec;
	//	outputVec.reserve(3);
	//	// 最初の行はスキップ
	//	getline(ifs, line);
	//	while (getline(ifs, line))
	//	{
	//		split(line, ',', outputVec);
	//		if (outputVec[0] == "")break;
	//		_expDataTable.emplace_back(ExpData(atoi(outputVec[1].c_str()), atoi(outputVec[2].c_str())));
	//	}
	//}

	// 属性テーブルの読み込み
	{
		_attributeDataTable.reserve(4);
		ifstream ifs("Resource/DataBase/Attribute.csv");
		string line;
		vector<string> outputVec;
		outputVec.reserve(4);
		// 最初の行はスキップ
		getline(ifs, line);
		while (getline(ifs, line))
		{
			split(line, ',', outputVec);
			if (outputVec[0] == "")break;
			_attributeDataTable.emplace_back(outputVec[1], atoi(outputVec[2].c_str()));
		}
	}

	// rateTableの読み込み
	{
		ifstream ifs("Resource/DataBase/AttributeRate.csv");
		string line;
		vector<string> outputVec;
		outputVec.reserve(5);

		// 最初の行は相手の属性名が書いてあるので保存しておく
		getline(ifs, line);
		int y = 0;
		_attributeRateTable.resize(_attributeDataTable.size());
		while (getline(ifs, line))
		{
			_attributeRateTable[y].resize(_attributeDataTable.size());
			split(line, ',', outputVec);
			if (outputVec[0] == "")break;
			for (size_t x = 0; x < outputVec.size()-1; x++)
			{
				_attributeRateTable[y][x] = static_cast<float>(atof(outputVec[x + 1].c_str()));
			}
			y++;
		}
	}

	// mapDataTableの読み込み
	{
		ifstream ifs("Resource/DataBase/Map.csv");
		string line;
		vector<string> outputVec;
		outputVec.reserve(10);
		// 最初の行はスキップ
		getline(ifs, line);
		while (getline(ifs, line))
		{
			split(line, ',', outputVec);
			if (outputVec[0] == "")break;
			_mapDataTable.emplace_back(MapData(outputVec[1], outputVec[2]));
		}
	}

	// weaponTypeDataTableの読み込み
	{
		ifstream ifs("Resource/DataBase/weaponType.csv");
		string line;
		vector<string> outputVec;
		outputVec.reserve(6);
		// 最初の行はスキップ
		getline(ifs, line);
		while (getline(ifs, line))
		{
			split(line, ',', outputVec);
			if (outputVec[0] == "")break;
			WeaponTypeData wt = { outputVec[1], outputVec[2] != "",
				outputVec[3] != "", atoi(outputVec[4].c_str()), outputVec[5] };
			_weaponTypeDataTable.emplace_back(wt);
		}
	}

	// weaponDataTableの読み込み
	{
		ifstream ifs("Resource/DataBase/weapon.csv");
		string line;
		vector<string> outputVec;
		outputVec.reserve(10);
		// 最初の行はスキップ
		getline(ifs, line);
		while (getline(ifs, line))
		{
			split(line, ',', outputVec);
			if (outputVec[0] == "")break;
			atoi(outputVec[8].c_str());
			WeaponData wd = {
				atoi(outputVec[1].c_str()), outputVec[2], 
				atoi(outputVec[3].c_str()),	atoi(outputVec[4].c_str()), atoi(outputVec[5].c_str()), atoi(outputVec[6].c_str()),
				Range(atoi(outputVec[7].c_str()), atoi(outputVec[8].c_str())), atoi(outputVec[9].c_str())};
			_weaponDataTable.emplace_back(wd);
		}
	}
}

DataBase::~DataBase()
{
}

float DataBase::GetAttributeRate(const uint8_t selfAttributeId, const uint8_t targetAttributeId) const
{
	return _attributeRateTable[selfAttributeId][targetAttributeId];
}

int DataBase::GetCharactorImageHandle(const CharactorType charactorType, const Team team) const
{
	char str[256];
	sprintf_s(str, 256, "%s%s", GetCharactorData(charactorType).ImagePath.c_str(), team == Team::player ? "_player.png" : "_enemy.png");
	return Application::Instance().GetFileSystem().GetImageHandle(str);
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
	status.level		  = level;
	status.health		 = status.health		+ static_cast<uint8_t>(level * charactorData.statusGrowRate.health			/ 100.0f);
	status.power		 = status.power			+ static_cast<uint8_t>(level * charactorData.statusGrowRate.power			/ 100.0f);
	status.magic_power	 = status.magic_power	+ static_cast<uint8_t>(level * charactorData.statusGrowRate.magic_power		/ 100.0f);
	status.defense		 = status.defense		+ static_cast<uint8_t>(level * charactorData.statusGrowRate.defense			/ 100.0f);
	status.magic_defense = status.magic_defense + static_cast<uint8_t>(level * charactorData.statusGrowRate.magic_defense	/ 100.0f);
	status.speed		 = status.speed			+ static_cast<uint8_t>(level * charactorData.statusGrowRate.speed			/ 100.0f);
	status.skill		 = status.skill			+ static_cast<uint8_t>(level * charactorData.statusGrowRate.skill			/ 100.0f);
	status.luck			 = status.luck			+ static_cast<uint8_t>(level * charactorData.statusGrowRate.luck			/ 100.0f);
	return status;
}

const DataBase::MapChipData& DataBase::GetMapChipData(const Map_Chip mapChip) const
{
	return _mapChipDataTable.at(static_cast<size_t>(mapChip));
}

//const DataBase::ExpData& DataBase::GetExpData(const uint8_t level) const
//{
//	return _expDataTable.at(level);
//}

const DataBase::AttributeData& DataBase::GetAttributeData(const uint8_t attibuteId) const
{
	return _attributeDataTable[attibuteId];
}

const std::vector<DataBase::MapData>& DataBase::GetMapDataTable() const
{
	return _mapDataTable;
}

const DataBase::MapData& DataBase::GetMapData(const unsigned int mapDataId) const
{
	return _mapDataTable.at(mapDataId);
}

const WeaponTypeData& DataBase::GetWeaponTypeData(const unsigned int weaponId) const
{
	return _weaponTypeDataTable[_weaponDataTable[weaponId].typeId];
}

const WeaponData& DataBase::GetWeaponData(const unsigned int weaponId) const
{
	return _weaponDataTable[weaponId];
}

const std::vector<WeaponData>& DataBase::GetWeaponDataTable() const
{
	return _weaponDataTable;
}
