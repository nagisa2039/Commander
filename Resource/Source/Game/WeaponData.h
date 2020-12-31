#pragma once
#include <string>
#include "Geometry.h"
#include "Effect/BattleEffect/BattleEffectType.h"

/// <summary>
/// �����f�[�^
/// </summary>
struct WeaponTypeData
{
	// ���O
	std::string name;
	// ���@�U����
	bool magicAttack;
	// �񕜂�
	bool heal;
	// ����
	uint8_t attribute;
	// �A�C�R���t�@�C���p�X
	std::string iconPath;

	/// <summary>
	/// �A�C�R���`��
	/// </summary>
	/// <param name="rect">�`���`</param>
	void DrawWeaponIcon(const Rect& rect)const;
};

/// <summary>
/// ����f�[�^
/// </summary>
struct WeaponData
{
	// ����ID
	uint8_t typeId;
	// ���O
	std::string name;
	// �U����
	uint8_t power;
	// ������
	uint8_t hit;
	// �K�E��
	uint8_t critical;
	// �d��
	uint8_t weight;
	// �U���͈�
	Range range;
	// �l�i
	unsigned int price;
	// �G�t�F�N�g��
	BattleEffectType effectType;
	
	/// <summary>
	/// �U���͈͂�String�ɕϊ��������̂�Ԃ�
	/// </summary>
	std::string GetRengeString()const;

	/// <summary>
	/// �����f�[�^��Ԃ�
	/// </summary>
	const WeaponTypeData& GetTypeData()const;
};