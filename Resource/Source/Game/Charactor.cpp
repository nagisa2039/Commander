#include "Charactor.h"
#include "MapCtrl.h"
#include "Camera.h"
#include "../Utility/DxLibUtility.h"
#include "Animator.h"
#include "../Scene/SceneController.h"
#include "../Scene/BattleScene.h"
#include "Effect.h"
#include <DxLib.h>
#include "Application.h"
#include "FlyText.h"
#include <algorithm>

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
	auto chipSize = _mapCtrl.GetChipSize();

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
	for (const auto& movePos : _resutlPosList)
	{
		Rect box(offset + (movePos.mapPos * chipSize.ToVector2Int() + chipSize*0.5) +-1, chipSize);
		unsigned int color = movePos.attack ? 0xff0000 : 0x0000ff;
		box.Draw(color);
		box.Draw(color, false);
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

Charactor::Status Charactor::GetStartStatus() const
{
	return _startStatus;
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

bool Charactor::GetIsMoveAnim() const
{
	return _isMoveAnim;
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

void Charactor::SetStatus(const Status& status)
{
	_status = status;
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

void Charactor::SearchAndMove()
{
	MoveMapPos(_mapCtrl.SearchMovePos(*this));
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
	_canMove = true;
	_isDying = false;

	_animator = make_shared<Animator>();

	_status = Status();
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
	: _selfChar(charactor), _size(128,128)
{
	auto wsize = Application::Instance().GetWindowSize();
	SetStartPos(Vector2());
	_animator = make_shared<Animator>();
	_uiSize = Size(wsize.w / 2, 200);

	_targetChar = nullptr;
}

BattleCharactor::~BattleCharactor()
{
}

void BattleCharactor::Init(const Vector2& startPos, const Dir dir, BattleCharactor* target)
{
	SetStartPos(startPos);
	SetDir(dir);
	SetTargetCharactor(target);

	_attackAnimCnt = -1;
	_attackAnimCntMax = 15;

	_createEffect = false;

	_animHealth = _selfChar.GetStatus().health;
	_animHealthCnt = 0;
}

void BattleCharactor::AnimUpdate()
{
	_animator->Update();
}

void BattleCharactor::AttackUpdate(BattleScene& battleScene)
{
	if(_attackAnimCnt >= 0)
	{
		Vector2 dir = _dir == Dir::left ? Vector2(1, 0) : Vector2(-1, 0);
		float per = _attackAnimCnt / static_cast<float>(_attackAnimCntMax);
		per = 1.0f - abs(0.5f - per)*2;
		_pos = Lerp(_startPos, _startPos + dir * 50, per);
		if (_attackAnimCnt++ >= _attackAnimCntMax)
		{
			_attackAnimCnt = -1;
		}
		if (!_createEffect && _attackAnimCnt >= _attackAnimCntMax / 2)
		{
			if (_targetChar != nullptr)
			{
				_createEffect = true;

				auto targetCenterPos = _targetChar->GetCenterPos();
				battleScene.GetEffectVec().emplace_back(CreateAttackEffect(targetCenterPos));

				int damage = _selfChar.GetStatus().GetDamage(_targetChar->GetSelfCharacotr().GetStatus());
				char damageText[10];
				sprintf_s(damageText, 10, "%d", damage);
				battleScene.GetEffectVec().emplace_back(make_shared<FlyText>(damageText, targetCenterPos, 60 * 1));
				_targetChar->AddDamage(damage);
			}
		}
	}
}

void BattleCharactor::Draw()
{
	_animator->Draw(GetDrawPos(_pos.ToVector2Int(), _size, Anker::centerdown), _size);
	UIDraw();
}

void BattleCharactor::UIAnimUpdate()
{
	uint8_t statusHp = _selfChar.GetStatus().health;
	if (statusHp == _animHealth)
	{
		return;
	}

	if (_animHealthCnt++ % 3 == 0)
	{
		_animHealth += _animHealth > statusHp ? -1 : 1;
	}
}

void BattleCharactor::UIDraw()
{
}

void BattleCharactor::StartAttackAnim()
{
	_attackAnimCnt = 0;
	_createEffect = false;
	_animHealth = _selfChar.GetStatus().health;
}

bool BattleCharactor::GetAttackAnimEnd()
{
	return _attackAnimCnt < 0;
}

void BattleCharactor::StartHPAnim()
{
	_animHealthCnt = 0;
}

bool BattleCharactor::GetHPAnimEnd()
{
	return _animHealth == _selfChar.GetStatus().health;
}

Size BattleCharactor::GetSize() const
{
	return _size;
}

Vector2Int BattleCharactor::GetCenterPos() const
{
	return Vector2Int(_pos.ToVector2Int() - Vector2Int(0, _size.h/2));
}

Charactor& BattleCharactor::GetSelfCharacotr()
{
	return _selfChar;
}

void BattleCharactor::SetStartPos(const Vector2& startPos)
{
	_startPos = startPos;
	_pos = startPos;
}

void BattleCharactor::SetTargetCharactor(BattleCharactor* target)
{
	_targetChar = target;
}

void BattleCharactor::AddDamage(const int damage)
{
	auto status = _selfChar.GetStatus();
	status.health -= damage;
	_selfChar.SetStatus(status);
}

int Charactor::Status::GetDamage(const Status& target)const
{
	return max(power - target.defense, 0);
}

int Charactor::Status::GetHit(const Status& target) const
{
	return max(100, 0);
}

int Charactor::Status::GetCritical(const Status& target) const
{
	return max(100, 0);
}
