#pragma once
#include <string>
#include "Geometry.h"
#include "Effect/BattleEffect/BattleEffectType.h"

/// <summary>
/// 武器種データ
/// </summary>
struct WeaponTypeData
{
	// 名前
	std::string name;
	// 魔法攻撃か
	bool magicAttack;
	// 回復か
	bool heal;
	// 属性
	uint8_t attribute;
	// アイコンファイルパス
	std::string iconPath;

	/// <summary>
	/// アイコン描画
	/// </summary>
	/// <param name="rect">描画矩形</param>
	void DrawWeaponIcon(const Rect& rect)const;
};

/// <summary>
/// 武器データ
/// </summary>
struct WeaponData
{
	// 武器ID
	uint8_t typeId;
	// 名前
	std::string name;
	// 攻撃力
	uint8_t power;
	// 命中率
	uint8_t hit;
	// 必殺率
	uint8_t critical;
	// 重さ
	uint8_t weight;
	// 攻撃範囲
	Range range;
	// 値段
	unsigned int price;
	// エフェクト種
	BattleEffectType effectType;
	
	/// <summary>
	/// 攻撃範囲をStringに変換したものを返す
	/// </summary>
	std::string GetRengeString()const;

	/// <summary>
	/// 武器種データを返す
	/// </summary>
	const WeaponTypeData& GetTypeData()const;
};