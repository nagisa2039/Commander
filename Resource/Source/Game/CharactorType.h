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
};
