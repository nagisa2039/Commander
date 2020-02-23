#pragma once
#include "Actor.h"
#include <list>
#include "Astar.h"
#include <array>
#include "../Utility/Dir.h"

class MapCtrl;

class Charactor :
	public Actor
{
public:
	enum class Team
	{
		Blue,
		Red,
		Green,
		Max
	};

	struct MoveInf
	{
		Dir dir;
		bool attack;

		MoveInf() : dir(Dir::left), attack(false) {};
		MoveInf(const Dir d, const bool at) : dir(d), attack(at) {};
	};

protected:
	Team _team;
	MapCtrl& _mapCtrl;
	std::list<Astar::ResultPos> _resutlPosList;

	bool isMoveAnim;	// 移動アニメーション中
	std::list<MoveInf> _moveDirList;
	std::array<Vector2Int, Dir::max> _dirTable;

	int _moveSpeed;
	bool _isSelect;

	void Move();

	unsigned int GetTeamColor()const;
	void DrawMovableMass(const Camera& camera)const;


public:
	Charactor(const Vector2Int& mapPos, const Team team, MapCtrl& mapCtrl);
	~Charactor();

	virtual void Update(const Input& input)override = 0;
	virtual void Draw(const Camera& camera)override = 0;
	Vector2Int GetMapPos()const;
	Team GetTeam()const;

	bool MoveMapPos(const Vector2Int& mapPos);

	bool GetIsSelect()const;
	void SetIsSelect(const bool select);
};

