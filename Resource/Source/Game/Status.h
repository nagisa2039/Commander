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

	unsigned int exp;		// 経験値
	unsigned int weaponId;	// 武器

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

	int defenseCorrection;		// 守備力補正
	int avoidanceCorrection;	// 回避力補正

	BattleStatus(const Status& st, const WeaponData& wd, const int dc, const int ac)
		: status(st), weaponData(wd), defenseCorrection(dc), avoidanceCorrection(ac) {};

	int GetPower()const;
	int GetDamage(const BattleStatus& target)const;	// ダメージ
	int GetRecover();	// 回復量
	int GetHitRate()const;	// 命中率
	int GetHit(const BattleStatus& target)const;	// 相手を考慮した命中率
	int GetCriticalRate()const;	// 必殺率
	int GetCritical(const BattleStatus& target)const;	// 相手を考慮した必殺率
	int GetAttackSpeed()const;	// 攻速
	int GetDifense()const;		// 防御力(補正込みの守備力)
	int GetMagicDifense()const;	// 耐魔(補正値込みの魔法守備力)
	int GetAvoidance()const;	// 回避(補正値込みの回避力)
	bool CheckHeal()const;		// 回復役か確認
	bool CheckMagicAttack()const;	// 魔法攻撃か確認

	bool CheckPursuit(const BattleStatus& target)const;	// 追撃が取れるか確認

};