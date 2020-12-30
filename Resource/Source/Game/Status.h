#pragma once
#include <cstdint>
#include <string>
#include "WeaponData.h"

/// <summary>
/// �X�e�[�^�X
/// </summary>
struct Status
{
	// ���x��
	uint8_t level;
	// �̗�
	uint8_t health;
	// ��
	uint8_t power;
	// ����
	uint8_t magic_power;
	// ���
	uint8_t defense;
	// ���h
	uint8_t magic_defense;
	// ����
	uint8_t speed;
	// �Z
	uint8_t skill;
	// �K�^
	uint8_t luck;
	// �ړ���
	uint8_t move;
	// �o���l
	uint8_t exp;
	// ����
	uint8_t weaponId;
};

struct WeaponData;

/// <summary>
/// �퓬���X�e�[�^�X
/// </summary>
struct BattleStatus
{
	// ���ƂȂ�X�e�[�^�X
	const Status& status;
	// ����f�[�^
	const WeaponData& weaponData;
	// �����f�[�^
	const WeaponTypeData& weaponTypeData;
	// ����͕␳
	int defenseCorrection;
	// ���͕␳
	int avoidanceCorrection;	
	
	/// <summary>
	/// �U���͎擾
	/// </summary>
	int GetPower()const;

	/// <summary>
	/// �^����_���[�W�ʎ擾
	/// </summary>
	/// <param name="target">�퓬����̃X�e�[�^�X</param>
	int GetDamage(const BattleStatus& target)const;

	/// <summary>
	/// �񕜗�
	/// </summary>
	int GetRecover();

	/// <summary>
	/// ������
	/// </summary>
	int GetHitRate()const;

	/// <summary>
	/// ������l������������
	/// </summary>
	/// <param name="target">�퓬����̃X�e�[�^�X</param>
	int GetHit(const BattleStatus& target)const;

	/// <summary>
	/// �K�E��
	/// </summary>
	int GetCriticalRate()const;

	/// <summary>
	/// ������l�������K�E��
	/// </summary>
	/// <param name="target">�퓬����̃X�e�[�^�X</param>
	int GetCritical(const BattleStatus& target)const;

	/// <summary>
	/// �U��
	/// </summary>
	int GetAttackSpeed()const;

	/// <summary>
	/// �h���(�␳���݂̎����)
	/// </summary>
	int GetDifense()const;

	/// <summary>
	/// �ϖ�(�␳�l���݂̖��@�����)
	/// </summary>
	int GetMagicDifense()const;

	/// <summary>
	/// ���(�␳�l���݂̉���)
	/// </summary>
	int GetAvoidance()const;

	/// <summary>
	/// �񕜖����m�F
	/// </summary>
	bool CheckHeal()const;

	/// <summary>
	/// ���@�U�����m�F
	/// </summary>
	bool CheckMagicAttack()const;

	/// <summary>
	/// �ǌ������邩�m�F
	/// </summary>
	/// <param name="target">�퓬����̃X�e�[�^�X</param>
	bool CheckPursuit(const BattleStatus& target)const;

};