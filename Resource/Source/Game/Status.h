#pragma once
#include <cstdint>
#include <string>
#include "WeaponData.h"

struct Status
{
	uint8_t level;
	uint8_t health;
	uint8_t power;
	uint8_t defense;
	uint8_t magic_defense;
	uint8_t speed;
	uint8_t skill;
	uint8_t move;

	unsigned int exp;		// �o���l
	unsigned int weaponId;	// ����

	Status() : level(1), health(1), power(1), defense(1), magic_defense(1), speed(1), skill(1), move(1), exp(0), weaponId(0) {};

	Status(const uint8_t lv, const uint8_t he, const uint8_t pw, const uint8_t df, const uint8_t md, const uint8_t sp, const uint8_t sk, const uint8_t mv)
		: level(lv), health(he), power(pw), defense(df), magic_defense(md), speed(sp), skill(sk), move(mv), exp(0), weaponId(0) {};

	void AddStatus(const Status& addStatus);
};

struct WeaponData;

struct BattleStatus
{
	const Status& status;
	const WeaponData& weaponData;

	int defenseCorrection;		// ����͕␳
	int avoidanceCorrection;	// ���͕␳

	BattleStatus(const Status& st, const WeaponData& wd, const int dc, const int ac)
		: status(st), weaponData(wd), defenseCorrection(dc), avoidanceCorrection(ac) {};

	int GetPower()const;
	int GetDamage(const BattleStatus& target)const;	// �_���[�W
	int GetRecover();	// �񕜗�
	int GetHitRate()const;	// ������
	int GetHit(const BattleStatus& target)const;	// ������l������������
	int GetCriticalRate()const;	// �K�E��
	int GetCritical(const BattleStatus& target)const;	// ������l�������K�E��
	int GetAttackSpeed()const;	// �U��
	int GetDifense()const;		// �h���(�␳���݂̎����)
	int GetMagicDifense()const;	// �ϖ�(�␳�l���݂̖��@�����)
	int GetAvoidance()const;	// ���(�␳�l���݂̉���)
	bool CheckHeal()const;		// �񕜖����m�F
	bool CheckMagicAttack()const;	// ���@�U�����m�F

	bool CheckPursuit(const BattleStatus& target)const;	// �ǌ������邩�m�F

};