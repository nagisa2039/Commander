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
#include "FileSystem.h"

using namespace std;

void Charactor::NormalUpdate()
{
	Move();

	_animator->ChangeAnim(_dirTable[_dir].animName);
	_animator->Update();
}

void Charactor::DyingUpdate()
{
	_dyingAnimAlphaTL->Update();
	if (_dyingAnimAlphaTL->GetEnd())
	{
		_delete = true;
	}
}

void Charactor::NormalDraw(const Camera& camera)
{
	auto offset = camera.GetCameraOffset();
	auto chipSize = _mapCtrl.GetChipSize().ToVector2Int();

	DrawMovableMass(camera);

	if (!_canMove)
	{
		SetDrawBright(128, 128, 128);
	}
	Vector2Int drawPos = offset + _pos.ToVector2Int();
	_animator->Draw(drawPos, _mapCtrl.GetChipSize());

	SetDrawBright(255, 255, 255);

	auto circleOffset = Vector2Int(0, -chipSize.y / 2);
	DrawCircle(circleOffset + drawPos + chipSize * 0.5, chipSize.x / 4, GetTeamColor(), true);

	// アイコンの描画
	int handle = Application::Instance().GetFileSystem()->GetImageHandle(_iconPath.c_str());
	DrawGraph(camera.GetCameraOffset() + _pos.ToVector2Int(), handle, false);

	// HPBerの描画
	Size hpberSize = (chipSize * Vector2(0.8f, 0.1f)).ToSize();
	Vector2Int hpLeftup		= drawPos + chipSize * Vector2(0.1f, 0.8f);
	Vector2Int hpRightdown	= hpLeftup + hpberSize;
	DrawBox(hpLeftup + Vector2Int::One() * -2, hpRightdown + Vector2Int::One() * +2, 0xffffff);
	DrawBox(hpLeftup + Vector2Int::One() * -1, hpRightdown + Vector2Int::One() * +1, 0xaaaaaa);
	float berLength = max(min(static_cast<float>(_status.health) / static_cast<float>(_startStatus.health), 1.0f), 0.0f);
	DrawBox(hpLeftup, hpLeftup + hpberSize.ToVector2Int() * Vector2(berLength, 1.0f), GetTeamColor());
}

void Charactor::DyingDraw(const Camera& camera)
{
	auto offset = camera.GetCameraOffset();

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * _dyingAnimAlphaTL->GetValue());
	_animator->Draw(offset + _pos.ToVector2Int(), _mapCtrl.GetChipSize());
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
}

void Charactor::Move()
{
	auto moveAnimEnd = [&]()
	{
		_isMoveAnim = false;
		RouteSearch();
	};

	if ((!_isMoveAnim || _moveDirList.size() == 0))
	{
		return;
	}

	auto it = _moveDirList.begin();
	_dir = it->dir;
	if (it->attack)
	{
		// そのマスに敵がいたら戦闘
		auto charactor = _mapCtrl.GetMapPosChar(it->mapPos);
		if (charactor != nullptr && _team != charactor->GetTeam())
		{
			// 戦闘
			_controller.PushScene(make_shared<BattleScene>(GetBattleC(), charactor->GetBattleC(), _controller));
			moveAnimEnd();
		}
		else
		{
			moveAnimEnd();
		}
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

		Vector2Int leftup = offset + movePos.mapPos * chipSize.ToVector2Int();
		DrawFormatString(leftup.x, leftup.y, 0x000000, "%d", movePos.moveCnt);
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

void Charactor::SetIsDying()
{
	_isDying = true;

	_updater = &Charactor::DyingUpdate;
	_drawer	 = &Charactor::DyingDraw;
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
	RouteSearch();
}

void Charactor::RouteSearch()
{
	_mapCtrl.RouteSearch(GetMapPos(), _status.move, _resutlPosList);
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

std::list<Astar::ResultPos>& Charactor::GetResutlPosList()
{
	return _resutlPosList;
}

void Charactor::AddDamage(const int damage)
{
	_status.health = max(_status.health - damage, 0);
	if (_status.health <= 0)
	{
		SetIsDying();
	}
	SetStatus(_status);
}

Charactor::Charactor(const uint8_t level, const Vector2Int& mapPos, const Team team, MapCtrl& mapCtrl, SceneController& ctrl,
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

	_dyingAnimAlphaTL = make_unique<TimeLine<float>>();
	_dyingAnimAlphaTL->AddKey(0, 1.0f);
	_dyingAnimAlphaTL->AddKey(60, 0.0f);

	_updater = &Charactor::NormalUpdate;
	_drawer = &Charactor::NormalDraw;

	//_mapCtrl.SearchMovePos(*this);
}

Charactor::~Charactor()
{
}

void Charactor::Update(const Input& input)
{
	(this->*_updater)();
}

void Charactor::Draw(const Camera& camera)
{
	(this->*_drawer)(camera);
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

	int range = 1;


	_moveDirList.clear();
	for (const auto& resultPos : _resutlPosList)
	{
		if (mapPos == resultPos.mapPos)
		{
			// _resultPosListからmoveDirListを作るために
			// 終わりから始まりへと辿ったリストを作る
			std::list<Astar::ResultPos> oneLineResutlList;
			oneLineResutlList.clear();
			oneLineResutlList.emplace_front(resultPos);
			Astar::ResultPos* rp = resultPos.parent;

			// attackマスが何マス続くかの数(攻撃範囲内かの確認のため)
			int attackMassCnt = 0;
			for(;rp->parent != nullptr;)
			{
				oneLineResutlList.emplace_front(*rp);
				if (rp->attack)
				{
					attackMassCnt++;
				}
				rp = rp->parent;
			}


			// 攻撃範囲外のマスを削除
			for (int i = range; i < attackMassCnt; i++)
			{
				oneLineResutlList.pop_back();
			}

			for (auto resutl : oneLineResutlList)
			{
				if (resutl.moveCnt <= _status.move || resutl.attack)
				{
					_moveDirList.emplace_back(MoveInf(resutl.dir, resutl.attack, resutl.mapPos));
				}
				else
				{
					break;
				}
			}

			_isMoveAnim = true;

			_status.move = max(_status.move - resultPos.moveCnt, 0);

			return true;
		}
	}
	return false;
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
