#pragma once
#include "Actor.h"
#include <list>
#include "Astar.h"
#include <array>
#include "../Utility/Dir.h"
#include "Team.h"

class Animator;
class MapCtrl;

class Charactor :
	public Actor
{
public:
	struct MoveInf
	{
		Dir dir;
		bool attack;

		MoveInf() : dir(Dir::left), attack(false) {};
		MoveInf(const Dir d, const bool at) : dir(d), attack(at) {};
	};

protected:
	struct DirInf
	{
		Vector2Int moveVec;
		std::string animName;

		DirInf() : moveVec(Vector2Int()), animName("") {};
		DirInf(const Vector2Int& vec2Int, const std::string& animN)
			: moveVec(vec2Int), animName(animN) {};
	};

	Team _team;
	MapCtrl& _mapCtrl;
	std::list<Astar::ResultPos> _resutlPosList;
	std::shared_ptr<Animator> _animator;

	bool isMoveAnim;	// 移動アニメーション中
	std::list<MoveInf> _moveDirList;
	std::array<DirInf, Dir::max> _dirTable;

	int _moveSpeed;
	bool _isSelect;
	Dir _dir;

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

