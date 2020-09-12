#pragma once
#include "CharactorType.h"

using Map_Chip = unsigned int;

namespace
{
	constexpr unsigned int MAP_CHIP_NONE = 0;
}

struct MapData
{
	Map_Chip mapChip = 0;
	CharactorChipInf charactorChip;
};