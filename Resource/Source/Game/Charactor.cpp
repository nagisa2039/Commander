#include "Charactor.h"
#include "MapCtrl.h"
#include "Camera.h"
#include "../Utility/DxLibUtility.h"

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

unsigned int Charactor::GetTeamColor() const
{
	switch (_team)
	{
	case Team::Red:
		return 0xff0000;
	case Team::Green:
		return 0x00ff00;
	case Team::Blue:
		return 0x0000ff;
	default:
		return 0xffffff;
	}
}

void Charactor::DrawMovableMass(const Camera& camera) const
{
	if (!_isSelect)
	{
		return;
	}

	auto offset = camera.GetCameraOffset();
	auto chipSize = _mapCtrl.GetChipSize().ToVector2Int();

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
	for (const auto& movePos : _resutlPosList)
	{
		DrawBox(offset + movePos.mapPos * chipSize, offset + (movePos.mapPos + 1) * chipSize, movePos.attack ? 0xff0000 : 0x0000ff);
	}
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
}

bool Charactor::GetIsSelect() const
{
	return _isSelect;
}

void Charactor::SetIsSelect(const bool select)
{
	_isSelect = select;
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
	_isSelect = false;

	_resutlPosList = _mapCtrl.RouteSearch(mapPos, 5);
}

Charactor::~Charactor()
{
}

Vector2Int Charactor::GetMapPos() const
{
	return _pos.ToVector2Int() / _mapCtrl.GetChipSize().ToVector2Int();
}

Team Charactor::GetTeam() const
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
