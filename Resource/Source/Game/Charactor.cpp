#include "Charactor.h"

Charactor::Charactor(const Vector2Int& mapPos, const Charactor::Team team):_mapPos(mapPos), _team(team)
{
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
