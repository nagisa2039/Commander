#pragma once
#include <string>
#include "Geometry.h"
#include "Effect/BattleEffect/BattleEffectType.h"

/// <summary>
/// ํํf[^
/// </summary>
struct WeaponTypeData
{
	// ผO
	std::string name;
	// @Uฉ
	bool magicAttack;
	// ๑ฉ
	bool heal;
	// ฎซ
	uint8_t attribute;
	// ACRt@CpX
	std::string iconPath;

	/// <summary>
	/// ACR`ๆ
	/// </summary>
	/// <param name="rect">`ๆ้`</param>
	void DrawWeaponIcon(const Rect& rect)const;
};

/// <summary>
/// ํf[^
/// </summary>
struct WeaponData
{
	// ํID
	uint8_t typeId;
	// ผO
	std::string name;
	// Uอ
	uint8_t power;
	// ฝฆ
	uint8_t hit;
	// KEฆ
	uint8_t critical;
	// dณ
	uint8_t weight;
	// Uออ
	Range range;
	// li
	unsigned int price;
	// GtFNgํ
	BattleEffectType effectType;
	
	/// <summary>
	/// Uออ๐Stringษฯทตฝเฬ๐ิท
	/// </summary>
	std::string GetRengeString()const;

	/// <summary>
	/// ํํf[^๐ิท
	/// </summary>
	const WeaponTypeData& GetTypeData()const;
};