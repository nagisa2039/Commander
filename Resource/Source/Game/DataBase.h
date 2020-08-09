#pragma once
#include <array>
#include <string>
#include <vector>
#include "../Utility/Geometry.h"
#include "Team.h"
#include "MapChip.h"
#include "WeaponData.h"
#include "SaveDataCharactor.h"

class DataBase
{
public:
	struct DrawData
	{
		Vector2Int leftup;
		Size size;
		std::string path;

		DrawData() {};

		DrawData(Vector2Int leftup, Size size, std::string path) :
			leftup(leftup), size(size), path(path) {};

	};

	struct MapChipData
	{
		DrawData drawData;
		int moveCost;
		int defense;
		int avoidance;
		int recovery;
		std::string name;
		unsigned int simpleColor;
	};

private:

	struct CharactorData
	{
		std::string name;
		uint8_t weaponType;
		Status initialStatus;
		Status statusGrowRate;
		std::string imagePath;
		std::string iconImagePath;

		void DrawIcon(const Rect& rect, const Team team)const;
	};

	//struct ExpData
	//{
	//	unsigned int maxPoint;	// 必要量
	//	unsigned int getPoint;	// 取得量
	//	ExpData() :maxPoint(0), getPoint(0) {};
	//	ExpData(unsigned int m, unsigned int g) :maxPoint(m), getPoint(g) {};
	//};

	struct AttributeData
	{
		std::string name;
		unsigned int color;
		AttributeData():name(""), color(0) {};
		AttributeData(const std::string& na, const unsigned int c) :name(na), color(c) {};

	};

	struct MapData
	{
		std::string name;
		std::string fileName;
		MapData() :name(""), fileName("") {};
		MapData(const std::string& na, const std::string& pa) :name(na), fileName(pa) {};
	};

	DataBase(const DataBase&) = delete;
	DataBase& operator=(const DataBase&) = delete;

	std::array<CharactorData, static_cast<size_t>(CharactorType::max)> _charactorDataTable;

	std::array<DataBase::MapChipData, static_cast<size_t>(Map_Chip::max)> _mapChipDataTable;

	//std::vector<ExpData> _expDataTable;

	std::vector<AttributeData> _attributeDataTable;
	std::vector<std::vector<float>> _attributeRateTable;

	std::vector<WeaponTypeData> _weaponTypeDataTable;
	std::vector<WeaponData> _weaponDataTable;

	std::vector<MapData> _mapDataTable;

	std::vector<SaveDataCharactor> _saveDataCharactors;	// セーブデータ生成用

	bool ReadData(const char* path, std::vector<std::vector<std::string>>& out);

public:
	DataBase();
	~DataBase();

	// 属性ごとの威力倍率を取得する		GetAttributeRate(攻撃する側,	攻撃される側)
	float GetAttributeRate(const uint8_t selfAttributeId, const uint8_t targetAttributeId)const;

	// キャラクターの画像を取得する
	int GetCharactorImageHandle(const CharactorType charactorType, const Team team)const;

	// キャラクターのﾃﾞｰﾀを取得する
	const CharactorData& GetCharactorData(const CharactorType charactorType)const;

	// キャラクタータイプとレベルを引数に期待値でStatusを作成する
	Status GetLevelInitStatus(const uint8_t level, const CharactorType charType)const;

	// マップチップデータを取得する
	const MapChipData& GetMapChipData(const Map_Chip mapChip)const;

	// 経験値データを取得する
	//const ExpData& GetExpData(const uint8_t level)const;

	// 属性データを取得する
	const AttributeData& GetAttributeData(const uint8_t attlebuteId)const;

	// マップデータを取得する
	const std::vector<MapData>& GetMapDataTable()const;
	const MapData& GetMapData(const unsigned int mapDataId)const;

	// WeaponTypeDataを取得する
	const WeaponTypeData& GetWeaponTypeData(const uint8_t type)const;
	const WeaponTypeData& GetWeaponTypeDataFromWeaponId(const uint8_t weaponId)const;

	// WeaponDataを取得する
	const WeaponData& GetWeaponData(const unsigned int weaponId)const;
	// weaponDataの配列取得
	const std::vector<WeaponData>& GetWeaponDataTable()const;

	// 初期のセーブデータ作成用配列の取得
	const std::vector<SaveDataCharactor> GetSaveDataCharactors()const;
};

