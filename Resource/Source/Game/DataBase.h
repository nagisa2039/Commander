#pragma once

#include <array>
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
	std::vector<std::vector<float>> _attributeRateTable;

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
		std::string path;
		MapData() :name(""), path("") {};
		MapData(const std::string& na, const std::string& pa) :name(na), path(pa) {};
	};

	std::array<CharactorData, static_cast<size_t>(CharactorType::max)> _charactorDataTable;

	std::array<MapChipData, static_cast<size_t>(Map_Chip::max)> _mapChipDataTable;

	std::vector<ExpData> _expDataTable;

	std::vector<AttributeData> _attributeDataTable;

	std::vector<MapData> _mapDataTable;

public:
	DataBase();
	~DataBase();

	// 属性ごとの威力倍率を取得する		GetAttributeRate(攻撃する側,	攻撃される側)
	float GetAttributeRate(const unsigned int selfAtributeId, const unsigned int targetAtributeId)const;
	// キャラクターの画像を取得する
	int GetCharactorImageHandle(const CharactorType charactorType, const Team team)const;
	// キャラクターのﾃﾞｰﾀを取得する
	const CharactorData& GetCharactorData(const CharactorType charactorType)const;
	// マップチップデータを取得する
	const MapChipData& GetMapChipData(const Map_Chip mapChip)const;
	// 経験値データを取得する
	const ExpData& GetExpData(const uint8_t level)const;
	// 属性データを取得する
	const AttributeData& GetAttributeData(const unsigned int attributeId)const;
	// マップデータを取得する
	const std::vector<MapData>& GetMapDataTable()const;
	const MapData& GetMapData(const unsigned int mapDataId)const;
};

