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

	std::vector<SaveDataCharactor> _saveDataCharactors;	// �Z�[�u�f�[�^�����p

	bool ReadData(const char* path, std::vector<std::vector<std::string>>& out);

public:
	DataBase();
	~DataBase();

	// �������Ƃ̈З͔{�����擾����		GetAttributeRate(�U�����鑤,	�U������鑤)
	float GetAttributeRate(const uint8_t selfAttributeId, const uint8_t targetAttributeId)const;

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
	const AttributeData& GetAttributeData(const uint8_t attlebuteId)const;

	// �}�b�v�f�[�^���擾����
	const std::vector<MapData>& GetMapDataTable()const;
	const MapData& GetMapData(const unsigned int mapDataId)const;

	// WeaponTypeData���擾����
	const WeaponTypeData& GetWeaponTypeData(const uint8_t type)const;
	const WeaponTypeData& GetWeaponTypeDataFromWeaponId(const uint8_t weaponId)const;

	// WeaponData���擾����
	const WeaponData& GetWeaponData(const unsigned int weaponId)const;
	// weaponData�̔z��擾
	const std::vector<WeaponData>& GetWeaponDataTable()const;

	// �����̃Z�[�u�f�[�^�쐬�p�z��̎擾
	const std::vector<SaveDataCharactor> GetSaveDataCharactors()const;
};

