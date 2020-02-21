#pragma once
#include "Actor.h"
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

public:
	Charactor(const Vector2Int& mapPos, const Team team);
	~Charactor();

	virtual void Update(const Input& input)override = 0;
	virtual void Draw(const Camera& camera)override = 0;
	Vector2Int GetMapPos()const;
	Team GetTeam()const;
};

