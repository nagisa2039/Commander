#pragma once
#include <cstdint>
#include <string>

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

	unsigned int exp;	// 経験値

	unsigned int weaponId;

	int defenseCorrection;		// 守備力補正
	int avoidanceCorrection;	// 回避力補正

	Status() : level(1), health(1), power(1), defense(1), magic_defense(1), speed(1), skill(1), move(1), exp(0), weaponId(0),
		defenseCorrection(0), avoidanceCorrection(0.0f) {};
	Status(const uint8_t lv, const uint8_t he, const uint8_t pw, const uint8_t df, const uint8_t md,
		const uint8_t sp, const uint8_t sk, const uint8_t mv, const unsigned int at, const bool ma, const bool heal)
		: level(lv), health(he), power(pw), defense(df), magic_defense(md), speed(sp), skill(sk), move(mv), exp(0), weaponId(0),
		defenseCorrection(0), avoidanceCorrection(0) {};

	int GetDamage(const Status& target)const;	// ダメージ
	int GetRecover();	// 回復量
	int GetHitRate()const;	// 命中率
	int GetHit(const Status& target)const;	// 相手を考慮した命中率
	int GetCriticalRate()const;	// 必殺率
	int GetCritical(const Status& target)const;	// 相手を考慮した必殺率
	int GetAttackSpeed()const;	// 攻速
	int GetDifense()const;		// 防御力(補正込みの守備力)
	int GetMagicDifense()const;	// 耐魔(補正値込みの魔法守備力)
	int GetAvoidance()const;	// 回避(補正値込みの回避力)
	bool CheckHeal()const;		// 回復役か確認
	bool CheckMagicAttack()const;	// 魔法攻撃か確認

	bool CheckPursuit(const Status& target)const;	// 追撃が取れるか確認

	void AddStatus(Status addStatus);
};