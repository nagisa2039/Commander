#pragma once
#include <cstdint>
#include "Attribute.h"

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

	Attribute attribute;		// ����
	bool magicAttack;	// ���@�U��
	bool heal;		// ��

	uint8_t defenseCorrection;		// ����͕␳
	uint8_t avoidanceCorrection;	// ���͕␳

	Status() : level(1), health(1), power(1), defense(1), magic_defense(1), speed(1), skill(1), move(1),
		attribute(Attribute::normal), defenseCorrection(0), avoidanceCorrection(0.0f) {};
	Status(const uint8_t lv, const uint8_t he, const uint8_t pw, const uint8_t df, const uint8_t md,
		const uint8_t sp, const uint8_t sk, const uint8_t mv, const Attribute at, const bool ma, const bool heal)
		: level(lv), health(he), power(pw), defense(df), magic_defense(md), speed(sp), skill(sk), move(mv),
		attribute(at), magicAttack(ma), heal(heal), defenseCorrection(0), avoidanceCorrection(0) {};

	int GetDamage(const Status& target)const;	// �_���[�W
	int GetHitRate()const;	// ������
	int GetHit(const Status& target)const;	// ������l������������
	int GetCriticalRate()const;	// �K�E��
	int GetCritical(const Status& target)const;	// ������l�������K�E��
	int GetAttackSpeed()const;	// �U��
	int GetDifense()const;		// �h���(�␳���݂̎����)
	int GetMagicDifense()const;	// �ϖ�(�␳�l���݂̖��@�����)
	int GetAvoidance()const;	// ���(�␳�l���݂̉���)
	bool CheckPursuit(const Status& target)const;	// �ǌ������邩�m�F
};