#pragma once

#include <array>
#include "Attribute.h"
#include <string>
#include "Geometry.h"
#include "Status.h"
#include "Team.h"
#include "CharactorType.h"
#include "MapChip.h"
#include <vector>

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
	std::array<std::array<float, static_cast<size_t>(Attribute::max)>, static_cast<size_t>(Attribute::max)> _attributeRateTable;

	//	テーブルに値を設定する 有利不利がセットで設定される	SetAttributeRateValue(攻撃する側,	攻撃される側, 有効か？)
	bool SetAttributeRateValue(const Attribute self, const Attribute target, const bool advantage);

	struct CharactorData
	{
		std::string name;
		Range range;
		Status initialStatus;
		Status statusGrowRate;
		std::string ImagePath;
		std::string iconImagePath;
	};

	struct ExpData
	{
		unsigned int maxPoint;	// 必要量
		unsigned int getPoint;	// 取得量
		ExpData() :maxPoint(0), getPoint(0) {};
		ExpData(unsigned int m, unsigned int g) :maxPoint(m), getPoint(g) {};
	};

	std::array<CharactorData, static_cast<size_t>(CharactorType::max)> _charactorDataTable;

	std::array<MapChipData, static_cast<size_t>(Map_Chip::max)> _mapChipDataTable;

	std::vector<ExpData> _expDataTable;

public:
	DataBase();
	~DataBase();

	// 属性ごとの威力倍率を取得する		GetAttributeRate(攻撃する側,	攻撃される側)
	float GetAttributeRate(const Attribute self, const Attribute target)const;

	int GetCharactorImageHandle(const CharactorType charactorType, const Team team)const;

	const CharactorData& GetCharactorData(const CharactorType charactorType)const;

	const MapChipData& GetMapChipData(const Map_Chip mapChip)const;

	const ExpData& GetExpData(const uint8_t level)const;
};

