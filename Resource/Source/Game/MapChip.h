#pragma once
#include "CharactorType.h"

using Map_Chip = unsigned int;

namespace
{
	// マップチップの0番をないものとして扱う
	constexpr unsigned int MAP_CHIP_NONE = 0;
}

/// <summary>
/// マップデータ
/// </summary>
struct MapData
{
	// マップチップ情報
	Map_Chip mapChip = 0;
	// キャラクターチップ情報
	CharactorChipInf charactorChip;
};