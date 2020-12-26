#pragma once
#include "Team.h"
#include "Geometry.h"

using CharactorType = unsigned int;

/// <summary>
/// キャラクター情報
/// </summary>
struct CharactorChipInf
{
	// マップ座標
	Vector2Int mapPos;
	// レベル
	unsigned int level;
	// グループ番号
	unsigned int groupNum;
	// 種類
	CharactorType type;
	// チーム
	Team team;
	//　アクティブ状態
	bool active = false;
	// 武器
	uint8_t weaponId;
	// 8バイト区切りにするための詰め物
	uint8_t stuffing[7];	
};
