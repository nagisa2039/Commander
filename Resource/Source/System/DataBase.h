#pragma once
#include <array>
#include <string>
#include <vector>
#include <memory>
#include "../Utility/Geometry.h"
#include "Team.h"
#include "MapChip.h"
#include "WeaponData.h"
#include "SaveDataCharactor.h"

class BattleEffectFactory;
class Map;

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
		DrawData drawData = {};
		int moveCost = 0;
		int defense = 0;
		int avoidance = 0;
		int recovery = 0;
		std::string name ="";
		unsigned int simpleColor=0;
	};

private:
	DataBase();
	DataBase(const DataBase&) = delete;
	DataBase& operator=(const DataBase&) = delete;

	struct CharactorData
	{
		std::string name;
		uint8_t weaponType = 0;
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

	struct BattleEffectData
	{
		std::string name;
		std::string seName;
	};

	std::vector<CharactorData> _charactorDataTable;

	std::vector<DataBase::MapChipData> _mapChipDataTable;

	//std::vector<ExpData> _expDataTable;

	std::vector<AttributeData> _attributeDataTable;
	std::vector<std::vector<float>> _attributeRateTable;

	std::vector<WeaponTypeData> _weaponTypeDataTable;
	std::vector<WeaponData> _weaponDataTable;

	std::vector<std::shared_ptr<Map>> _mapTable;

	std::vector<SaveDataCharactor> _saveDataCharactors;	// �Z�[�u�f�[�^�����p

	std::vector<BattleEffectData> _battleEffectDataTable;
	std::unique_ptr<BattleEffectFactory> _battleEffectFactory;

	std::vector<std::string> _advanceLoadTable;	// ���O�Ǎ����郊�\�[�X�����i�[

	bool ReadData(const char* path, std::vector<std::vector<std::string>>& out);

public:
	~DataBase();

	static DataBase& Instance()
	{
		static DataBase instance;
		return instance;
	}

	void Init();

	// �������Ƃ̈З͔{�����擾����		GetAttributeRate(�U�����鑤,	�U������鑤)
	float GetAttributeRate(const uint8_t selfAttributeId, const uint8_t targetAttributeId)const;

	// �L�����N�^�[�̉摜���擾����
	int GetCharactorImageHandle(const CharactorType charactorType, const Team team)const;

	// �L�����N�^�[���ް����擾����
	const CharactorData& GetCharactorData(const CharactorType charactorType)const;

	/// <summary>
	/// �L�����N�^�[�̃f�[�^�e�[�u�����擾
	/// </summary>
	const std::vector<CharactorData>& GetCharactorDataTable()const;

	// �L�����N�^�[�^�C�v�ƃ��x���������Ɋ��Ғl��Status���쐬����
	Status GetLevelInitStatus(const uint8_t level, const CharactorType charType)const;

	// �}�b�v�`�b�v�f�[�^���擾����
	const MapChipData& GetMapChipData(const Map_Chip mapChip)const;

	/// <summary>
	/// �}�b�v�`�b�v�̃f�[�^�e�[�u�����擾
	/// </summary>
	const std::vector<MapChipData>& GetMapChipDataTable()const;

	// �o���l�f�[�^���擾����
	//const ExpData& GetExpData(const uint8_t level)const;

	// �����f�[�^���擾����
	const AttributeData& GetAttributeData(const uint8_t attlebuteId)const;

	// �}�b�v�f�[�^���擾����
	const std::vector<std::shared_ptr<Map>>& GetMapDataTable()const;
	const std::shared_ptr<Map> GetMapData(const unsigned int mapDataId)const;

	// WeaponTypeData���擾����
	const WeaponTypeData& GetWeaponTypeData(const uint8_t type)const;
	const WeaponTypeData& GetWeaponTypeDataFromWeaponId(const uint8_t weaponId)const;

	// WeaponData���擾����
	const WeaponData& GetWeaponData(const unsigned int weaponId)const;
	// weaponData�̔z��擾
	const std::vector<WeaponData>& GetWeaponDataTable()const;

	// �����̃Z�[�u�f�[�^�쐬�p�z��̎擾
	const std::vector<SaveDataCharactor> GetSaveDataCharactors()const;

	/// <summary>
	/// �U���G�t�F�N�g�����N���X���擾
	/// </summary>
	const BattleEffectFactory& GetBattleEffectFactory()const;

	/// <summary>
	/// BattleEffectData�̎擾
	/// </summary>
	const BattleEffectData& GetBattleEffectData(const BattleEffectType type)const;

	/// <summary>
	/// ���O�Ǎ����郊�\�[�X�p�X�擾
	/// </summary>
	const std::vector<std::string>& GetAdvanceLoadTable()const;
};

