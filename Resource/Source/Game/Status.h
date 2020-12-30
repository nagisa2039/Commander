#pragma once
#include <cstdint>
#include <string>
#include "WeaponData.h"

/// <summary>
/// ステータス
/// </summary>
struct Status
{
	// レベル
	uint8_t level;
	// 体力
	uint8_t health;
	// 力
	uint8_t power;
	// 魔力
	uint8_t magic_power;
	// 守備
	uint8_t defense;
	// 魔防
	uint8_t magic_defense;
	// 速さ
	uint8_t speed;
	// 技
	uint8_t skill;
	// 幸運
	uint8_t luck;
	// 移動力
	uint8_t move;
	// 経験値
	uint8_t exp;
	// 武器
	uint8_t weaponId;
};

struct WeaponData;

/// <summary>
/// 戦闘時ステータス
/// </summary>
struct BattleStatus
{
	// 元となるステータス
	const Status& status;
	// 武器データ
	const WeaponData& weaponData;
	// 武器種データ
	const WeaponTypeData& weaponTypeData;
	// 守備力補正
	int defenseCorrection;
	// 回避力補正
	int avoidanceCorrection;	
	
	/// <summary>
	/// 攻撃力取得
	/// </summary>
	int GetPower()const;

	/// <summary>
	/// 与えるダメージ量取得
	/// </summary>
	/// <param name="target">戦闘相手のステータス</param>
	int GetDamage(const BattleStatus& target)const;

	/// <summary>
	/// 回復量
	/// </summary>
	int GetRecover();

	/// <summary>
	/// 命中率
	/// </summary>
	int GetHitRate()const;

	/// <summary>
	/// 相手を考慮した命中率
	/// </summary>
	/// <param name="target">戦闘相手のステータス</param>
	int GetHit(const BattleStatus& target)const;

	/// <summary>
	/// 必殺率
	/// </summary>
	int GetCriticalRate()const;

	/// <summary>
	/// 相手を考慮した必殺率
	/// </summary>
	/// <param name="target">戦闘相手のステータス</param>
	int GetCritical(const BattleStatus& target)const;

	/// <summary>
	/// 攻速
	/// </summary>
	int GetAttackSpeed()const;

	/// <summary>
	/// 防御力(補正込みの守備力)
	/// </summary>
	int GetDifense()const;

	/// <summary>
	/// 耐魔(補正値込みの魔法守備力)
	/// </summary>
	int GetMagicDifense()const;

	/// <summary>
	/// 回避(補正値込みの回避力)
	/// </summary>
	int GetAvoidance()const;

	/// <summary>
	/// 回復役か確認
	/// </summary>
	bool CheckHeal()const;

	/// <summary>
	/// 魔法攻撃か確認
	/// </summary>
	bool CheckMagicAttack()const;

	/// <summary>
	/// 追撃が取れるか確認
	/// </summary>
	/// <param name="target">戦闘相手のステータス</param>
	bool CheckPursuit(const BattleStatus& target)const;

};