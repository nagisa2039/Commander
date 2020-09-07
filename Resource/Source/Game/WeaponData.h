#pragma once
#include <string>
#include "Geometry.h"
#include "Effect/BattleEffect/BattleEffectType.h"

struct WeaponTypeData
{
	std::string name;
	bool magicAttack;
	bool heal;
	uint8_t attribute; 
	std::string iconPath;

	void DrawWeaponIcon(const Rect& rect)const;
};

struct WeaponData
{
	uint8_t typeId;
	std::string name;
	uint8_t power;
	uint8_t hit;
	uint8_t critical;
	uint8_t weight;
	Range range;
	unsigned int price;
	BattleEffectType effectType;
	
	std::string GetRengeString()const;
	const WeaponTypeData& GetTypeData()const;
};