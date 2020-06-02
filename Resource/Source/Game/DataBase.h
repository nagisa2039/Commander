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
		unsigned int maxPoint;	// •K—v—Ê
		unsigned int getPoint;	// æ“¾—Ê
		ExpData() :maxPoint(0), getPoint(0) {};
		ExpData(unsigned int m, unsigned int g) :maxPoint(m), getPoint(g) {};
	};

	struct AttributeData
	{
		std::string name;
		unsigned int color;
		AttributeData():name(""), color(0) {};
		AttributeData(const std::string na, const unsigned int c) :name(na), color(c) {};
	};

	std::array<CharactorData, static_cast<size_t>(CharactorType::max)> _charactorDataTable;

	std::array<MapChipData, static_cast<size_t>(Map_Chip::max)> _mapChipDataTable;

	std::vector<ExpData> _expDataTable;

	std::vector<AttributeData> _attributeDataTable;

public:
	DataBase();
	~DataBase();

	// ‘®«‚²‚Æ‚ÌˆĞ—Í”{—¦‚ğæ“¾‚·‚é		GetAttributeRate(UŒ‚‚·‚é‘¤,	UŒ‚‚³‚ê‚é‘¤)
	float GetAttributeRate(const unsigned int selfAtributeId, const unsigned int targetAtributeId)const;

	int GetCharactorImageHandle(const CharactorType charactorType, const Team team)const;

	const CharactorData& GetCharactorData(const CharactorType charactorType)const;

	const MapChipData& GetMapChipData(const Map_Chip mapChip)const;

	const ExpData& GetExpData(const uint8_t level)const;

	const AttributeData& GetAttributeData(const unsigned int attributeId)const;
};

