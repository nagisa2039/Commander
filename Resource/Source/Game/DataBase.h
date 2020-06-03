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
		unsigned int maxPoint;	// �K�v��
		unsigned int getPoint;	// �擾��
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

	// �������Ƃ̈З͔{�����擾����		GetAttributeRate(�U�����鑤,	�U������鑤)
	float GetAttributeRate(const unsigned int selfAtributeId, const unsigned int targetAtributeId)const;
	// �L�����N�^�[�̉摜���擾����
	int GetCharactorImageHandle(const CharactorType charactorType, const Team team)const;
	// �L�����N�^�[���ް����擾����
	const CharactorData& GetCharactorData(const CharactorType charactorType)const;
	// �}�b�v�`�b�v�f�[�^���擾����
	const MapChipData& GetMapChipData(const Map_Chip mapChip)const;
	// �o���l�f�[�^���擾����
	const ExpData& GetExpData(const uint8_t level)const;
	// �����f�[�^���擾����
	const AttributeData& GetAttributeData(const unsigned int attributeId)const;
	// �}�b�v�f�[�^���擾����
	const std::vector<MapData>& GetMapDataTable()const;
	const MapData& GetMapData(const unsigned int mapDataId)const;
};

