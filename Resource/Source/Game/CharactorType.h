#pragma once
#include "Team.h"
#include "Geometry.h"

enum class CharactorType
{
	none,
	swordman,
	soldier,
	warrior,
	mage,
	archer,
	max
};

struct CharactorChipInf
{
	Vector2Int mapPos;
	unsigned int level;
	CharactorType type;
	Team team;
};
