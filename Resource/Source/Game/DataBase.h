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
#include "WeaponData.h"

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
		Status initialStatus;
		Status statusGrowRate;
		std::string ImagePath;
		std::string iconImagePath;

		CharactorData() {};
	};

	//struct ExpData
	//{
	//	unsigned int maxPoint;	// �K�v��
	//	unsigned int getPoint;	// �擾��
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

	std::array<CharactorData, static_cast<size_t>(CharactorType::max)> _charactorDataTable;

	std::array<MapChipData, static_cast<size_t>(Map_Chip::max)> _mapChipDataTable;

	//std::vector<ExpData> _expDataTable;

	std::unordered_map<std::string, AttributeData> _attributeDataTable;
	std::unordered_map<std::string, std::unordered_map<std::string, float>> _attributeRateTable;

	std::vector<WeaponData> _weaponDataTable;

	std::vector<MapData> _mapDataTable;

public:
	DataBase();
	~DataBase();

	// �������Ƃ̈З͔{�����擾����		GetAttributeRate(�U�����鑤,	�U������鑤)
	float GetAttributeRate(const std::string& atributeName, const std::string& targetAtributeName)const;

	// �L�����N�^�[�̉摜���擾����
	int GetCharactorImageHandle(const CharactorType charactorType, const Team team)const;

	// �L�����N�^�[���ް����擾����
	const CharactorData& GetCharactorData(const CharactorType charactorType)const;

	// �L�����N�^�[�^�C�v�ƃ��x���������Ɋ��Ғl��Status���쐬����
	Status GetLevelInitStatus(const uint8_t level, const CharactorType charType)const;

	// �}�b�v�`�b�v�f�[�^���擾����
	const MapChipData& GetMapChipData(const Map_Chip mapChip)const;

	// �o���l�f�[�^���擾����
	//const ExpData& GetExpData(const uint8_t level)const;

	// �����f�[�^���擾����
	const AttributeData& GetAttributeData(const std::string& atributeName)const;

	// �}�b�v�f�[�^���擾����
	const std::vector<MapData>& GetMapDataTable()const;
	const MapData& GetMapData(const unsigned int mapDataId)const;

	// WeaponData���擾����
	const WeaponData& GetWeaponData(const unsigned int weaponId)const;

	const std::vector<WeaponData>& GetWeaponDataTable()const;
};

