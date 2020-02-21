#pragma once
#include "Actor.h"
#include <list>
#include "Astar.h"

class MapCtrl;

class Charactor :
	public Actor
{
public:
	enum class Team
	{
		Team_Blue,
		Team_Red,
		Team_Green,
		TeamMax
	};

protected:
	Vector2Int _mapPos;
	Team _team;
	MapCtrl& _mapCtrl;
	std::list<Astar::ResultPos> resutlPosList;

public:
	Charactor(const Vector2Int& mapPos, const Team team, MapCtrl& mapCtrl);
	~Charactor();

	virtual void Update(const Input& input)override = 0;
	virtual void Draw(const Camera& camera)override = 0;
	Vector2Int GetMapPos()const;
	Team GetTeam()const;
};

