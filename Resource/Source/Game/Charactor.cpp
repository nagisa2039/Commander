#include "Charactor.h"
#include "MapCtrl.h"

Charactor::Charactor(const Vector2Int& mapPos, const Team team, MapCtrl& mapCtrl):_mapPos(mapPos), _team(team), _mapCtrl(mapCtrl)
{
	_pos = (_mapPos * _mapCtrl.GetChipSize().ToIntVector()).ToFloatVector();
}

Charactor::~Charactor()
{
}

Vector2Int Charactor::GetMapPos() const
{
	return _mapPos;
}

Charactor::Team Charactor::GetTeam() const
{
	return _team;
}
