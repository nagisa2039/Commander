#pragma once
#include "CharactorType.h"

enum class Map_Chip
{
	none,
	forest,
	river_pond,
	river_vertical,
	river_horizontal,
	river_cross,
	river_all,
	river_corner0,
	river_corner1,
	river_corner2,
	river_corner3,
	rock,
	fort,
	lava,
	max
};

struct MapData
{
	Map_Chip mapChip = Map_Chip::none;
	CharactorChipInf charactorChip;
};