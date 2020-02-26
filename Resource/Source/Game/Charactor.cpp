#include "Charactor.h"
#include "MapCtrl.h"
#include "Camera.h"
#include "../Utility/DxLibUtility.h"
#include "Animator.h"
#include "../Scene/SceneController.h"
#include "../Scene/BattleScene.h"
#include "Effect.h"
#include <DxLib.h>

using namespace std;

void Charactor::Move()
{
	if (!_isMoveAnim || _moveDirList.size() == 0)
	{
		return;
	}

	auto moveAnimEnd = [&]()
	{
		_isMoveAnim = false;
		RouteSearch();
	};

	auto it = _moveDirList.begin();
	_dir = it->dir;
	if (it->attack)
	{
		// ÇªÇÃÉ}ÉXÇ…ìGÇ™Ç¢ÇΩÇÁêÌì¨
		auto charactor = _mapCtrl.GetMapPosChar(GetMapPos() + _dirTable[it->dir].moveVec);
		if (charactor != nullptr && _team != charactor->GetTeam())
		{
			// êÌì¨
			_controller.PushScene(make_shared<BattleScene>(GetBattleC(), charactor->GetBattleC(), _controller));
			MoveEnd();
		}
		moveAnimEnd();
		return;
	}

	_pos += (_dirTable[it->dir].moveVec * _moveSpeed).ToVector2();
	if (_pos.ToVector2Int() % _mapCtrl.GetChipSize().ToVector2Int() == Vector2Int(0, 0))
	{
		_moveDirList.pop_front();
		if (_moveDirList.size() <= 0)
		{
			moveAnimEnd();
		}
	}
}

unsigned int Charactor::GetTeamColor() const
{
	switch (_team)
	{
	case Team::player:
		return 0x0000ff;
	case Team::enemy:
		return 0xff0000;
	default:
		return 0xffffff;
	}
}

void Charactor::DrawMovableMass(const Camera& camera) const
{
	if (!_isSelect || !_canMove)
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

bool Charactor::GetCanMove() const
{
	return _canMove;
}

Charactor::Status Charactor::GetStatus() const
{
	return _status;
}

bool Charactor::GetIsDying() const
{
	return _isDying;
}

Dir Charactor::GetDir() const
{
	return _dir;
}

Vector2Int Charactor::GetCenterPos() const
{
	return Vector2Int(_pos.ToVector2Int() - (_mapCtrl.GetChipSize() - (_animator->GetAnimRect().size*0.5)).ToVector2Int());
}

BattleCharactor& Charactor::GetBattleC() const
{
	return *_battleC;
}

void Charactor::SetIsSelect(const bool select)
{
	_isSelect = select;
}

void Charactor::SetIsDying(const bool dying)
{
	_isDying = dying;
}

void Charactor::SetDir(const Dir dir)
{
	_dir = dir;
}

void Charactor::MoveEnd()
{
	_canMove = false;
	_isMoveAnim = false;
}

void Charactor::RouteSearch()
{
	_resutlPosList = _mapCtrl.RouteSearch(GetMapPos(), _status.move);
}

void Charactor::TurnReset()
{
	_status.move = _startStatus.move;
	_canMove = true;
}

Charactor::Charactor(const Vector2Int& mapPos, const Team team, MapCtrl& mapCtrl, SceneController& ctrl, 
	std::vector<std::shared_ptr<Effect>>& effects)
	: _team(team), _mapCtrl(mapCtrl), _controller(ctrl), _effects(effects)
{
	_pos = (mapPos * _mapCtrl.GetChipSize().ToVector2Int()).ToVector2();
	_isMoveAnim = false;

	_dirTable[Dir::left].moveVec	= Vector2Int(-1,0);
	_dirTable[Dir::right].moveVec	= Vector2Int(1, 0);
	_dirTable[Dir::up].moveVec		= Vector2Int(0, -1);
	_dirTable[Dir::down].moveVec	= Vector2Int(0, 1);

	_moveSpeed = 4;
	_isSelect = false;
	_canMove = false;
	_isDying = false;;

	_animator = make_shared<Animator>();
}

Charactor::~Charactor()
{
}

void Charactor::AnimRestart()
{
	_animator->AnimRestart();
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
	if (mapPos == GetMapPos() || _isMoveAnim)
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
			_isMoveAnim = true;
			_status.move -= resultPos.moveCnt;
			return true;
		}
	}
	return false;
}

BattleCharactor::BattleCharactor(Charactor& charactor)
	: _charactor(charactor), _size(128,128)
{
	SetStartPos(Vector2());
	_animator = make_shared<Animator>();
}

BattleCharactor::~BattleCharactor()
{
}

void BattleCharactor::Update(const Input& input)
{
	_animator->Update();
}

void BattleCharactor::Draw(const Camera& camera)
{
	_animator->Draw(GetDrawPos(_pos.ToVector2Int(), _size, Anker::centerdown), _size);
}

Size BattleCharactor::GetSize() const
{
	return _size;
}

Vector2Int BattleCharactor::GetCenterPos() const
{
	return Vector2Int(_pos.ToVector2Int() - Vector2Int(0, _size.h/2));
}

void BattleCharactor::SetStartPos(const Vector2& startPos)
{
	_startPos = startPos;
	_pos = startPos;
}