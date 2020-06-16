#pragma once

#include <array>
#include <string>
#include "Geometry.h"
#include "Status.h"
#include "Team.h"
#include "CharactorType.h"
#include "MapChip.h"
#include <vector>
#include <unordered_map>

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

	struct WeaponData
	{
		std::string name;
		unsigned int power;
		unsigned int hit;
		unsigned int critical;
		unsigned int weight;
		Range range;
		bool magicAttack;
		bool heal;
		std::string atribute;
		std::string iconPath;

		//WeaponData& operator=(const WeaponData&) = default;

		WeaponData() : name(""), power(0), hit(0), critical(0), weight(0), range(0, 0), magicAttack(false), heal(false), atribute(""), iconPath("") {};
		WeaponData(const std::string na, const unsigned int pw, const unsigned int ht, const unsigned int ct, const unsigned int we,
			const Range& ra, const bool ma, const bool he, const std::string& atribute, const std::string& icon)
			: name(na), power(pw), hit(ht), critical(ct), weight(we), range(ra), magicAttack(ma), heal(he), atribute(atribute), iconPath(icon) {};
	};

private:

	struct CharactorData
	{
		std::string name;
		Status initialStatus;
		Status statusGrowRate;
		std::string ImagePath;
		std::string iconImagePath;

		CharactorData() {};
	};

	struct ExpData
	{
		unsigned int maxPoint;	// 必要量
		unsigned int getPoint;	// 取得量
		ExpData() :maxPoint(0), getPoint(0) {};
		ExpData(unsigned int m, unsigned int g) :maxPoint(m), getPoint(g) {};
	};

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

	std::array<CharactorData, static_cast<size_t>(CharactorType::max)> _charactorDataTable;

	std::array<MapChipData, static_cast<size_t>(Map_Chip::max)> _mapChipDataTable;

	std::vector<ExpData> _expDataTable;

	std::unordered_map<std::string, AttributeData> _attributeDataTable;
	std::unordered_map<std::string, std::unordered_map<std::string, float>> _attributeRateTable;

	std::vector<WeaponData> _weaponDataTable;

	std::vector<MapData> _mapDataTable;

public:
	DataBase();
	~DataBase();

	// 属性ごとの威力倍率を取得する		GetAttributeRate(攻撃する側,	攻撃される側)
	float GetAttributeRate(const std::string& atributeName, const std::string& targetAtributeName)const;
	// キャラクターの画像を取得する
	int GetCharactorImageHandle(const CharactorType charactorType, const Team team)const;
	// キャラクターのﾃﾞｰﾀを取得する
	const CharactorData& GetCharactorData(const CharactorType charactorType)const;
	// キャラクタータイプとレベルを引数に期待値でStatusを作成する
	Status GetLevelInitStatus(const uint8_t level, const CharactorType charType)const;
	// マップチップデータを取得する
	const MapChipData& GetMapChipData(const Map_Chip mapChip)const;
	// 経験値データを取得する
	const ExpData& GetExpData(const uint8_t level)const;
	// 属性データを取得する
	const AttributeData& GetAttributeData(const std::string& atributeName)const;
	// マップデータを取得する
	const std::vector<MapData>& GetMapDataTable()const;
	const MapData& GetMapData(const unsigned int mapDataId)const;

	// WeaponDataを取得する
	const WeaponData& GetWeaponData(const unsigned int weaponId)const;
};

