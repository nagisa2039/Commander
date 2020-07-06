#pragma once
#include "Team.h"
#include "Geometry.h"

enum class CharactorType
{
	swordman,
	soldier,
	warrior,
	mage,
	archer,
	priest,
	max

};

struct CharactorChipInf
{
	Vector2Int mapPos;
	unsigned int level;
	unsigned int groupNum;
	CharactorType type;
	Team team;
	bool active;

	uint8_t weaponId;	// 武器

	uint8_t stuffing[3];	// 4バイト区切りにするための詰め物
};
