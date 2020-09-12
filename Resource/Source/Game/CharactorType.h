#pragma once
#include "Team.h"
#include "Geometry.h"

using CharactorType = unsigned int;

struct CharactorChipInf
{
	Vector2Int mapPos;
	unsigned int level;
	unsigned int groupNum;
	CharactorType type;
	Team team;
	bool active = false;

	uint8_t weaponId;	// ����

	uint8_t stuffing[7];	// 8�o�C�g��؂�ɂ��邽�߂̋l�ߕ�
};
