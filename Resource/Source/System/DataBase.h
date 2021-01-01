#pragma once
#include <array>
#include <string>
#include <vector>
#include <memory>
#include "../Utility/Geometry.h"
#include "Team.h"
#include "MapChip.h"
#include "WeaponData.h"
#include "Status.h"

class BattleEffectFactory;
class Map;

/// <summary>
/// �f�[�^�x�[�X�Ǘ��N���X
/// </summary>
class DataBase
{
public:
	/// <summary>
	/// �`��f�[�^
	/// </summary>
	struct DrawData
	{ 
		// ������W
		Vector2Int leftup;
		// �`��T�C�Y
		Size size;
		// �摜�p�X
		std::string path;
	};

	/// <summary>
	/// �}�b�v�`�b�v���
	/// </summary>
	struct MapChipData
	{
		// �`��f�[�^
		DrawData drawData = {};
		// �ړ��R�X�g
		int moveCost = 0;
		// �h��␳�l
		int defense = 0;
		// ���␳�l
		int avoidance = 0;
		// �񕜗�
		int recovery = 0;
		// �\����
		std::string name ="";
		// �틵�m�F��ʂŕ\������Ƃ��̐F
		unsigned int simpleColor=0;
	};

private:
	DataBase();
	DataBase(const DataBase&) = delete;
	DataBase& operator=(const DataBase&) = delete;

	/// <summary>
	/// �L�����N�^�[���
	/// </summary>
	struct CharactorData
	{
		// �\����
		std::string name;
		// �����
		uint8_t weaponType = 0;
		// �����X�e�[�^�X
		Status initialStatus = {};
		// �X�e�[�^�X������
		Status statusGrowRate = {};
		// �摜�t�@�C���p�X
		std::string imagePath;
		// �A�C�R���摜�p�X
		std::string iconImagePath;

		/// <summary>
		/// �A�C�R���`��
		/// </summary>
		/// <param name="rect">�\����`</param>
		/// <param name="team">�`�[��</param>
		void DrawIcon(const Rect& rect, const Team team)const;
	};

	/// <summary>
	/// �����f�[�^
	/// </summary>
	struct AttributeData
	{
		// ������
		std::string name;
		// �\���F
		unsigned int color;
	};

	/// <summary>
	/// �U���G�t�F�N�g�f�[�^
	/// </summary>
	struct BattleEffectData
	{
		// �G�t�F�N�g��
		std::string name;
		// SE�t�@�C���p�X
		std::string seName;
	};

	// �L�����N�^�[�f�[�^�e�[�u��
	std::vector<CharactorData> _charactorDataTable;
	// �}�b�v�`�b�v�f�[�^�e�[�u��
	std::vector<DataBase::MapChipData> _mapChipDataTable;
	// �����f�[�^�e�[�u��
	std::vector<AttributeData> _attributeDataTable;
	// ���������f�[�^�e�[�u��
	std::vector<std::vector<float>> _attributeRateTable;
	// �����f�[�^�e�[�u��
	std::vector<WeaponTypeData> _weaponTypeDataTable;
	// ����f�[�^�e�[�u��
	std::vector<WeaponData> _weaponDataTable;
	// �}�b�v�f�[�^�e�[�u��
	std::vector<std::shared_ptr<Map>> _mapTable;
	// �퓬�G�t�F�N�g�f�[�^�e�[�u��
	std::vector<BattleEffectData> _battleEffectDataTable;

	// �퓬�G�t�F�N�g�����N���X
	std::unique_ptr<BattleEffectFactory> _battleEffectFactory;

	// ���O�Ǎ����郊�\�[�X�����i�[
	std::vector<std::string> _advanceLoadTable;

	/// <summary>
	/// DATA�t�@�C��(CSV���o�C�i������������)�Ǎ�
	/// </summary>
	/// <param name="path">�t�@�C���p</param>
	/// <param name="out">���������؂�2�����z��Ɋi�[</param>
	void ReadData(const char* path, std::vector<std::vector<std::string>>& out);

public:
	~DataBase();

	/// <summary>
	/// DataBase�̎Q�Ǝ擾
	/// </summary>
	static DataBase& Instance()
	{
		static DataBase instance;
		return instance;
	}

	/// <summary>
	/// ������
	/// </summary>
	void Init();

	/// <summary>
	/// �������Ƃ̈З͔{�����擾����
	/// </summary>
	/// <param name="selfAttributeId">�U�����鑤,</param>
	/// <param name="targetAttributeId">�U������鑤</param>
	float GetAttributeRate(const uint8_t selfAttributeId, const uint8_t targetAttributeId)const;

	/// <summary>
	/// �L�����N�^�[�̉摜�n���h�����擾����
	/// </summary>
	/// <param name="charactorType">�L�����N�^�[��</param>
	/// <param name="team">�`�[��</param>
	int GetCharactorImageHandle(const CharactorType charactorType, const Team team)const;

	/// <summary>
	/// �L�����N�^�[�̃f�[�^���擾����
	/// </summary>
	/// <param name="charactorType">�L�����N�^�[��</param>
	const CharactorData& GetCharactorData(const CharactorType charactorType)const;

	/// <summary>
	/// �L�����N�^�[�̃f�[�^�e�[�u�����擾
	/// </summary>
	const std::vector<CharactorData>& GetCharactorDataTable()const;

	/// <summary>
	/// �^�C�v�ƃ��x���������Ɋ��Ғl�ŃX�e�[�^�X���쐬����
	/// </summary>
	/// <param name="level">���x��</param>
	/// <param name="charType">�L�����N�^�[��</param>
	Status GetLevelInitStatus(const uint8_t level, const CharactorType charType)const;

	/// <summary>
	/// �}�b�v�`�b�v�f�[�^���擾����
	/// </summary>
	/// <param name="mapChip">�}�b�v�`�b�v</param>
	const MapChipData& GetMapChipData(const Map_Chip mapChip)const;

	/// <summary>
	/// �}�b�v�`�b�v�̃f�[�^�e�[�u�����擾
	/// </summary>
	const std::vector<MapChipData>& GetMapChipDataTable()const;

	/// <summary>
	/// �����f�[�^���擾����
	/// </summary>
	/// <param name="attlebuteId">����ID</param>
	const AttributeData& GetAttributeData(const uint8_t attlebuteId)const;

	/// <summary>
	/// �}�b�v�f�[�^���擾����
	/// </summary>
	const std::vector<std::shared_ptr<Map>>& GetMapDataTable()const;

	/// <summary>
	/// �}�b�v�f�[�^���擾
	/// </summary>
	/// <param name="mapDataId">�}�b�v�f�[�^ID</param>
	const std::shared_ptr<Map> GetMapData(const unsigned int mapDataId)const;

	/// <summary>
	/// WeaponTypeData���擾����
	/// </summary>
	/// <param name="type">�����</param>
	const WeaponTypeData& GetWeaponTypeData(const uint8_t type)const;

	/// <summary>
	/// ����ID���畐���̎擾
	/// </summary>
	/// <param name="weaponId">����ID</param>
	const WeaponTypeData& GetWeaponTypeDataFromWeaponId(const uint8_t weaponId)const;

	/// <summary>
	/// WeaponData���擾����
	/// </summary>
	/// <param name="weaponId">����ID</param>
	const WeaponData& GetWeaponData(const unsigned int weaponId)const;
	/// <summary>
	/// weaponData�̔z��擾
	/// </summary>
	const std::vector<WeaponData>& GetWeaponDataTable()const;

	/// <summary>
	/// �U���G�t�F�N�g�����N���X���擾
	/// </summary>
	const BattleEffectFactory& GetBattleEffectFactory()const;

	/// <summary>
	/// BattleEffectData�̎擾
	/// </summary>
	/// <param name="type">�퓬�G�t�F�N�g��</param>
	const BattleEffectData& GetBattleEffectData(const BattleEffectType type)const;

	/// <summary>
	/// ���O�Ǎ����郊�\�[�X�p�X�擾
	/// </summary>
	const std::vector<std::string>& GetAdvanceLoadTable()const;
};

