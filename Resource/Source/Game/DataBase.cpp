#include "DataBase.h"
#include <fstream>
#include <sstream>
#include <Dxlib.h>
#include "../System/Application.h"
#include "../System/FileSystem.h"

using namespace std;

namespace
{
	std::string WStringToString(std::wstring inWstr)
	{
		// wstring → SJIS
		int iBufferSize = WideCharToMultiByte(CP_OEMCP, 0, inWstr.c_str()
			, -1, (char*)NULL, 0, NULL, NULL);

		// バッファの取得
		CHAR* cpMultiByte = new CHAR[iBufferSize];

		// wstring → SJIS
		WideCharToMultiByte(CP_OEMCP, 0, inWstr.c_str(), -1, cpMultiByte
			, iBufferSize, NULL, NULL);

		// stringの生成
		std::string oRet(cpMultiByte, cpMultiByte + iBufferSize - 1);

		// バッファの破棄
		delete[] cpMultiByte;

		// 変換結果を返す
		return(oRet);
	}
}

bool DataBase::ReadData(const char* path, std::vector<std::vector<std::string>>& outstrVec2)
{
	int fileH = FileRead_open(path);
	assert(fileH != 0);

	unsigned int lineCnt;
	FileRead_read(&lineCnt, sizeof(lineCnt), fileH);
	outstrVec2.resize(lineCnt);

	for (auto& outstr : outstrVec2)
	{
		unsigned int contentCnt;
		FileRead_read(&contentCnt, sizeof(contentCnt), fileH);
		outstr.resize(contentCnt);

		for (auto& out : outstr)
		{
			std::wstring content;
			unsigned int strLen;
			FileRead_read(&strLen, sizeof(strLen), fileH);

			content.resize(strLen);
			FileRead_read(&content[0],
				sizeof(content[0]) * strLen, fileH);

			out = WStringToString(content);
		}
	}
	FileRead_close(fileH);

	return true;
}

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
		vector<vector<string>> data;
		ReadData("Resource/DataBase/Charactor.data", data);
		int aryNum = 0;
		for (int idx = 1; idx < data.size(); ++idx)
		{
			CharactorData charactorData;
			// キャラクターID
			// 職業名
			charactorData.name = data[idx][1];

			// 武器種ID
			charactorData.weaponType = atoi(data[idx][2].c_str());

			// 初期ステータス
			int initSbeginIdx = 3;
			auto To_uint8_t = [&data,idx](const int& targetIdx)
			{
				return atoi(data[idx][static_cast<int>(targetIdx)].c_str());
			};
			charactorData.initialStatus = 
				Status(1, To_uint8_t(initSbeginIdx), To_uint8_t(initSbeginIdx+1), To_uint8_t(initSbeginIdx+2),
					To_uint8_t(initSbeginIdx+3), To_uint8_t(initSbeginIdx+4), To_uint8_t(initSbeginIdx+5),
					To_uint8_t(initSbeginIdx+6), To_uint8_t(initSbeginIdx+7), To_uint8_t(initSbeginIdx+8));
		
			// ステータス成長率
			int growSbeginIdx = 12;
			charactorData.statusGrowRate =
				Status(1, To_uint8_t(growSbeginIdx), To_uint8_t(growSbeginIdx + 1), To_uint8_t(growSbeginIdx + 2),
					To_uint8_t(growSbeginIdx + 3), To_uint8_t(growSbeginIdx + 4), To_uint8_t(growSbeginIdx + 5),
					To_uint8_t(growSbeginIdx + 6), To_uint8_t(growSbeginIdx + 7), 0);

			// キャラクター画像パス
			charactorData.imagePath = data[idx][20];
			// 職業アイコンパス
			charactorData.iconImagePath = data[idx][21];

			_charactorDataTable[aryNum++] = charactorData;
		}
	}

	// マップチップテーブルの読み込み
	{
		vector<vector<string>> data;
		ReadData("Resource/DataBase/MapChip.data", data);
		int aryNum = 0;
		for (int idx = 1; idx < data.size(); ++idx)
		{
			MapChipData mapChipData;
			// 名前
			mapChipData.name = data[idx][1];
			// パス
			mapChipData.drawData.path = data[idx][2];
			// 画像の左上座標
			mapChipData.drawData.leftup = Vector2Int(atoi(data[idx][3].c_str()), atoi(data[idx][4].c_str()));
			// 画像のサイズ
			mapChipData.drawData.size = Size(atoi(data[idx][5].c_str()), atoi(data[idx][6].c_str()));
			// 簡略化色
			mapChipData.simpleColor = atoi(data[idx][7].c_str());
			// 移動コスト
			mapChipData.moveCost = atoi(data[idx][8].c_str());
			// 防御補正値
			mapChipData.defense = atoi(data[idx][9].c_str());
			// 回避補正値
			mapChipData.avoidance = atoi(data[idx][10].c_str());
			// 回復量
			mapChipData.recovery = atoi(data[idx][11].c_str());

			_mapChipDataTable[aryNum] = mapChipData;
			aryNum++;
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
		vector<vector<string>> data;
		ReadData("Resource/DataBase/Attribute.data", data);
		for (int idx = 1; idx < data.size(); ++idx)
		{
			_attributeDataTable.emplace_back(data[idx][1], atoi(data[idx][2].c_str()));
		}
	}

	// rateTableの読み込み
	{
		vector<vector<string>> data;
		ReadData("Resource/DataBase/AttributeRate.data", data);

		_attributeRateTable.resize(_attributeDataTable.size());
		for(int y = 0; y < _attributeDataTable.size(); ++y)
		{
			_attributeRateTable[y].resize(_attributeDataTable.size());
			for (size_t x = 0; x < _attributeDataTable.size(); x++)
			{
				_attributeRateTable[y][x] = static_cast<float>(atof(data[static_cast<size_t>(y)+1][x + 1].c_str()));
			}
		}
	}

	// mapDataTableの読み込み
	{
		vector<vector<string>> data;
		ReadData("Resource/DataBase/map.data", data);
		_mapDataTable.resize(data.size());
		for (int idx = 1;auto& mapData : _mapDataTable)
		{
			mapData = MapData(data[idx][1], data[idx][2],data[idx][3]);
		}
	}

	// weaponTypeDataTableの読み込み
	{
		vector<vector<string>> data;
		ReadData("Resource/DataBase/weaponType.data", data);

		for (int idx = 1; idx < data.size(); ++idx)
		{
			WeaponTypeData wt = { data[idx][1], data[idx][2] != "",
				data[idx][3] != "", static_cast<uint8_t>(atoi(data[idx][4].c_str())), data[idx][5] };
			_weaponTypeDataTable.emplace_back(wt);
		}
	}

	// weaponDataTableの読み込み
	{
		vector<vector<string>> data;
		ReadData("Resource/DataBase/weapon.data", data);
		_weaponDataTable.reserve(data.size());
		for (int idx = 1; idx < data.size(); ++idx)
		{
			WeaponData wd = {
				static_cast<uint8_t>(atoi(data[idx][1].c_str())), data[idx][2],
				static_cast<uint8_t>(atoi(data[idx][3].c_str())), static_cast<uint8_t>(atoi(data[idx][4].c_str())),
				static_cast<uint8_t>(atoi(data[idx][5].c_str())), static_cast<uint8_t>(atoi(data[idx][6].c_str())),
				Range(atoi(data[idx][7].c_str()), atoi(data[idx][8].c_str())), static_cast<unsigned int>(atoi(data[idx][9].c_str()))};
			_weaponDataTable.emplace_back(wd);
		}
	}

	// キャラクターデータテーブルの読み込み
	{
		vector<vector<string>> data;
		ReadData("Resource/DataBase/StartPlayerCharactor.data", data);
		_saveDataCharactors.reserve(6);
		for (int idx = 1; idx < data.size(); ++idx)
		{
			auto charactorType = static_cast<CharactorType>(atoi(data[idx][1].c_str()));
			auto status = GetLevelInitStatus(atoi(data[idx][2].c_str()), charactorType);
			status.weaponId = atoi(data[idx][3].c_str());
			_saveDataCharactors.emplace_back(charactorType, status);
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
	sprintf_s(str, 256, "%s%s", GetCharactorData(charactorType).imagePath.c_str(), team == Team::player ? "_player.png" : "_enemy.png");
	return ImageHandle(str);
}

const DataBase::CharactorData& DataBase::GetCharactorData(const CharactorType charactorType) const
{
	return _charactorDataTable.at(static_cast<size_t>(charactorType));
}

Status DataBase::GetLevelInitStatus(const uint8_t level, const CharactorType charType)const
{
	Status status;
	auto charactorData = _charactorDataTable[static_cast<size_t>(charType)];
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

const WeaponTypeData& DataBase::GetWeaponTypeData(const uint8_t type) const
{
	return _weaponTypeDataTable[type];
}

const WeaponTypeData& DataBase::GetWeaponTypeDataFromWeaponId(const uint8_t weaponId) const
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

const std::vector<SaveDataCharactor> DataBase::GetSaveDataCharactors() const
{
	return _saveDataCharactors;
}

void DataBase::CharactorData::DrawIcon(const Rect& rect, const Team team)const
{
	SetDrawMode(DX_DRAWMODE_NEAREST);
	stringstream ss;
	ss << imagePath << "_" << (Team::player == team ? "player" : "enemy") << ".png";
	auto handle = ImageHandle(ss.str().c_str());
	rect.DrawRectGraph(Vector2Int(32, 0), Size(32, 32), handle);
	SetDrawMode(DX_DRAWMODE_BILINEAR);
	
}
