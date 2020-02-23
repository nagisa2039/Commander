#include "Charactor.h"
#include "MapCtrl.h"

void Charactor::Move()
{
	if (!isMoveAnim)
	{
		return;
	}

	if (_moveDirList.size() == 0)
	{
		isMoveAnim = false;
		_resutlPosList = _mapCtrl.RouteSearch(GetMapPos(), 5);
		return;
	}

	auto it = _moveDirList.begin();
	if (it->attack)
	{
		isMoveAnim = false;
		_resutlPosList = _mapCtrl.RouteSearch(GetMapPos(), 5);
		return;
	}

	_pos += (_dirTable[it->dir] * _moveSpeed).ToFloatVector();
	if (_pos.ToVector2Int() % _mapCtrl.GetChipSize().ToVector2Int() == Vector2Int(0, 0))
	{
		_moveDirList.pop_front();
	}
}

Charactor::Charactor(const Vector2Int& mapPos, const Team team, MapCtrl& mapCtrl): _team(team), _mapCtrl(mapCtrl)
{
	_pos = (mapPos * _mapCtrl.GetChipSize().ToVector2Int()).ToFloatVector();
	isMoveAnim = false;

	_dirTable[Dir::left] = Vector2Int(-1,0);
	_dirTable[Dir::right] = Vector2Int(1, 0);
	_dirTable[Dir::up] = Vector2Int(0, -1);
	_dirTable[Dir::down] = Vector2Int(0, 1);

	_moveSpeed = 4;

	_resutlPosList = _mapCtrl.RouteSearch(mapPos, 5);
}

Charactor::~Charactor()
{
}

Vector2Int Charactor::GetMapPos() const
{
	return _pos.ToVector2Int() / _mapCtrl.GetChipSize().ToVector2Int();
}

Charactor::Team Charactor::GetTeam() const
{
	return _team;
}

bool Charactor::MoveMapPos(const Vector2Int& mapPos)
{
	if (mapPos == GetMapPos() || isMoveAnim)
	{
		return false;
	}

	_moveDirList.clear();
	for (const auto& resultPos : _resutlPosList)
	{
		if (mapPos == resultPos.mapPos)
		{
			_moveDirList.emplace_front(MoveInf(resultPos.dir, resultPos.attack));

			Astar::ResultPos* rp = resultPos.parent;
			for(;rp->parent != nullptr;)
			{
				_moveDirList.emplace_front(MoveInf(rp->dir, rp->attack));
				rp = rp->parent;
			}
			isMoveAnim = true;
			return true;
		}
	}
	return false;
}
