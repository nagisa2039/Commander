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
#include "CorsorTarget.h"
#include "DataBase.h"

using namespace std;

void Charactor::NormalUpdate(const Input& input)
{
	if (_moveStandby)
	{
		if (--_rigid > 0)return;

		_moveStandby = false; 
		SearchAndMove();
	}

	Move();

	_animator->ChangeAnim(_dirTable[_dir].animName);
	_animator->Update();
}

void Charactor::BattaleStartUpdate(const Input& input)
{
	if (!_battleStartEffect->GetDelete())return;

	auto charactor = _mapCtrl.GetMapPosChar((*_moveDirList.begin()).mapPos);
	BattaleStart(charactor);

	_updater = &Charactor::NormalUpdate;
}

void Charactor::BattaleStart(Charactor* charactor)
{
	_mapCtrl.SetGroupActive(_groupNum, true);
	_mapCtrl.SetGroupActive(charactor->GetGroupNum(), true);

	// 戦闘
	_controller.PushScene(make_shared<BattleScene>
		(GetBattleC(), charactor->GetBattleC(), _controller, _camera.GetCameraOffset()));
}

void Charactor::DyingUpdate(const Input& input)
{
	_dyingAnimAlphaTL->Update();
	if (_dyingAnimAlphaTL->GetEnd())
	{
		_delete = true;
	}
}

void Charactor::NormalDraw()
{
	auto offset = _camera.GetCameraOffset();
	auto chipSize = _mapCtrl.GetChipSize().ToVector2Int();

	if (!_canMove)
	{
		SetDrawBright(128, 128, 128);
	}
	Vector2Int drawPos = offset + _pos.ToVector2Int();
	_animator->Draw(drawPos, _mapCtrl.GetChipSize());

	SetDrawBright(255, 255, 255);

	// アイコンの描画
	int handle = Application::Instance().GetFileSystem()->GetImageHandle(_iconPath.c_str());
	DrawGraph(_camera.GetCameraOffset() + _pos.ToVector2Int(), handle, false);

	// HPBerの描画
	Size hpberSize = (chipSize * Vector2(0.8f, 0.1f)).ToSize();
	Vector2Int hpLeftup		= drawPos + chipSize * Vector2(0.1f, 0.8f);
	Vector2Int hpRightdown	= hpLeftup + hpberSize;
	DrawBox(hpLeftup + Vector2Int::One() * -2, hpRightdown + Vector2Int::One() * +2, 0xffffff);
	DrawBox(hpLeftup + Vector2Int::One() * -1, hpRightdown + Vector2Int::One() * +1, 0xaaaaaa);
	float berLength = max(min(static_cast<float>(_status.health) / static_cast<float>(_startStatus.health), 1.0f), 0.0f);
	DrawBox(hpLeftup, hpLeftup + hpberSize.ToVector2Int() * Vector2(berLength, 1.0f), GetTeamColor());

	// レベルの描画
	DrawFormatString(drawPos.x, drawPos.y, 0x000000, "Level.%d", _status.level);
}

void Charactor::DyingDraw()
{
	auto offset = _camera.GetCameraOffset();

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * _dyingAnimAlphaTL->GetValue());
	_animator->Draw(offset + _pos.ToVector2Int(), _mapCtrl.GetChipSize());
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
}

void Charactor::Move()
{
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
		if (charactor != nullptr)
		{
			if (_team == Team::player)
			{
				BattaleStart(charactor);
				return;
			}
			_battleStartEffect->SetPos(charactor->GetCenterPos());
			_battleStartEffect->Reset();
			_effects.emplace_back(_battleStartEffect);
			_updater = &Charactor::BattaleStartUpdate;
			return;
		}
		else
		{
			MoveEnd(_status.move > 0);
		}
		return;
	}

	_pos += (_dirTable[it->dir].moveVec * _moveSpeed).ToVector2();
	if (_pos.ToVector2Int() % _mapCtrl.GetChipSize().ToVector2Int() == Vector2Int(0, 0))
	{
		_moveDirList.pop_front();
		if (_moveDirList.size() <= 0)
		{
			MoveEnd(_status.move > 0);
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

void Charactor::DrawMovableMass(const uint8_t alpha) const
{
	auto offset = _camera.GetCameraOffset();
	auto chipSize = _mapCtrl.GetChipSize();

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);

	for (size_t y = 0; y < _resultPosListVec2.size(); y++)
	{
		for (size_t x = 0; x < _resultPosListVec2[y].size(); x++)
		{
			if (_resultPosListVec2[y][x].size() <= 0)continue;

			Vector2Int mapPos(x,y);
			Rect box(offset + (mapPos * chipSize.ToVector2Int() + chipSize * 0.5) + -1, chipSize);

			unsigned int color = CheckMoveMapPos(mapPos) ? 0x0000ff : (_status.heal ? 0x00ff00 : 0xff0000);
			box.Draw(color);
			box.Draw(color, false);

			Vector2Int leftup = offset + mapPos * chipSize.ToVector2Int();
		}
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

bool Charactor::GetMoveActive() const
{
	return _moveActive;
}

bool Charactor::GetMoveStandby() const
{
	return _moveStandby;
}

Status Charactor::GetStartStatus() const
{
	return _startStatus;
}

Status Charactor::GetStatus() const
{
	Status status = _status;

	MapCtrl::MapChipData mapchipData = _mapCtrl.GetMapChipData(GetMapPos());
	status.avoidanceCorrection = mapchipData.avoidance;
	status.defenseCorrection = mapchipData.defense;

	return status;
}

bool Charactor::GetIsDying() const
{
	return _isDying;
}

Dir Charactor::GetDir() const
{
	return _dir;
}

Vector2 Charactor::GetCenterPos() const
{
	return Vector2(_pos + (_mapCtrl.GetChipSize() * 0.5f).ToVector2());
}

BattleCharactor& Charactor::GetBattleC() const
{
	return *_battleC;
}

bool Charactor::GetIsMoveAnim() const
{
	return _isMoveAnim;
}

Range Charactor::GetAttackRange() const
{
	return _attackRange;
}

const std::string& Charactor::GetName() const
{
	return _name;
}

const std::list<Astar::ResultPos>& Charactor::GetResutlPosList(const Vector2Int& mapPos)
{
	return _resultPosListVec2[mapPos.y][mapPos.x];
}

unsigned int Charactor::GetGroupNum() const
{
	return _groupNum;
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

void Charactor::SetMoveActive(const bool active)
{
	_moveActive = active;
}

void Charactor::SetMoveStandby(const int time)
{
	_moveStandby = true;
	_rigid = time;
}

void Charactor::MoveEnd(const bool canMove)
{
	if (!canMove)
	{
		_status.move = _startStatus.move;
	}
	_canMove = canMove;
	_isMoveAnim = false;
	_camera.PopTargetActor();

	_mapCtrl.AllCharactorRouteSearch();
}

void Charactor::RouteSearch()
{
	_mapCtrl.RouteSearch(*this);
}

void Charactor::TurnReset()
{
	_canMove = true;
}

void Charactor::SearchAndMove()
{
	MoveMapPos(_mapCtrl.SearchMovePos(*this));
}

std::vector<std::vector<std::list<Astar::ResultPos>>>& Charactor::GetResutlPosListVec2()
{
	return _resultPosListVec2;
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

void Charactor::DrawCharactorIcon(const Rect& drawRect)const
{
	DrawRectExtendGraph(drawRect.Left(), drawRect.Top(), drawRect.Right(), drawRect.Botton(), 32, 0, 32, 32, _animator->GetImageH(), true);
}

void Charactor::DrawRoute(const Vector2Int& targetPos)
{
	if (targetPos == GetMapPos()) return;
	if (_resultPosListVec2[targetPos.y][targetPos.x].size() <= 0)return;

	bool onCharactor = _mapCtrl.GetMapPosChar(targetPos) != nullptr;

	

	bool begin = true;
	for (const auto& route : CreateResultPosList(targetPos))
	{
		if (begin && route.attack)continue;

		auto offset = _camera.GetCameraOffset();
		auto chipSize = _mapCtrl.GetChipSize();
		size_t dir_idx = static_cast<size_t>(route.dir);

		if (0 > dir_idx || dir_idx >= _dirTable.size())return;

		Vector2Int endPos = offset + (route.prev == nullptr ? GetMapPos() : route.prev->mapPos) * chipSize + chipSize * 0.5f;
		Vector2Int startPos = offset + route.mapPos * chipSize + chipSize * 0.5f;
		DrawLine(startPos, endPos, 0xffff00, 10);
		if (begin)
		{
			auto arrowH = Application::Instance().GetFileSystem()->GetImageHandle("Resource/Image/UI/arrow.png");
			Size arrowSize;
			GetGraphSize(arrowH, arrowSize);
			DrawRotaGraph(startPos, chipSize.w / static_cast<float>(arrowSize.w), _dirTable[dir_idx].angle, arrowH, true);
			begin = false;
		}
	}

}

bool Charactor::CheckMoveMapPos(const Vector2Int mapPos) const
{
	for (const auto& resutlPos : _resultPosListVec2[mapPos.y][mapPos.x])
	{
		if (!resutlPos.attack)
		{
			return true;
		}
	}
	return false;
}

bool Charactor::CheckAttackMapPos(const Vector2Int mapPos) const
{
	for (const auto& resutlPos : _resultPosListVec2[mapPos.y][mapPos.x])
	{
		if (resutlPos.attack)
		{
			return true;
		}
	}
	return false;
}

std::list<Astar::ResultPos> Charactor::CreateResultPosList(const Vector2Int mapPos) const
{
	std::list<Astar::ResultPos> routeList;
	routeList.clear();
	// 移動可能なルートが無い
	if (_resultPosListVec2[mapPos.y][mapPos.x].size() <= 0)return routeList;

	Astar::ResultPos startResultPos = *_resultPosListVec2[mapPos.y][mapPos.x].begin();
	auto targetCharactor = _mapCtrl.GetMapPosChar(mapPos);
	if (targetCharactor != nullptr)
	{
		// 相手の攻撃範囲
		auto targetRange = targetCharactor->GetAttackRange();
		// 一方的に攻撃できる距離
		Range criticalRange = _attackRange.GetCriticalRange(targetRange);
		// 一方的に攻撃できる距離があるか
		if (criticalRange != Range(0, 0))
		{
			for (const auto& targetPos : _resultPosListVec2[mapPos.y][mapPos.x])
			{
				Vector2Int startPos = targetPos.prev == nullptr ? GetMapPos() : targetPos.prev->mapPos;
				Vector2Int disVec = targetPos.mapPos - startPos;
				unsigned int distance = abs(disVec.x) + abs(disVec.y);
				if (criticalRange.Hit(distance))
				{
					startResultPos = targetPos;
					break;
				}
			}
		}
	}
	else
	{
		// 敵がいないマスへの移動なので移動マスを優先的に採用する
		for (const auto& targetPos : _resultPosListVec2[mapPos.y][mapPos.x])
		{
			if (!targetPos.attack)
			{
				startResultPos = targetPos;
				break;
			}
		}
	}

	routeList.emplace_back(startResultPos);
	for (auto prev = startResultPos.prev; prev->prev != nullptr; prev = prev->prev)
	{
		routeList.emplace_back(*prev);
	}
	return routeList;
}

void Charactor::CreateMoveDirList(const std::list<Astar::ResultPos>& resultPosList)
{
	_moveDirList.clear();

	vector<vector<list<Astar::ResultPos>>> addResultPosListVec2;
	// 攻撃開始地点に敵が居ないかを確認するためのフラグ
	bool coverCheck = true;
	for (auto itr = resultPosList.begin(); itr != resultPosList.end(); itr++)
	{
		// 移動力を超えるマスは無視する
		if (itr->moveCnt > _status.move) continue;

		if (itr->attack)
		{
			_moveDirList.emplace_front(MoveInf(itr->dir, itr->attack, itr->mapPos));
			continue;
		}


		if (!coverCheck)
		{
			_moveDirList.emplace_front(MoveInf(itr->dir, itr->attack, itr->mapPos));
			continue;
		}

		if (_mapCtrl.GetMapPosChar(itr->mapPos) == nullptr)
		{
			coverCheck = false;
			_moveDirList.emplace_front(MoveInf(itr->dir, itr->attack, itr->mapPos));
		}
		else
		{
			for (auto& addResultPosListVec : addResultPosListVec2)
			{
				for (auto& addResultPosList : addResultPosListVec)
				{
					addResultPosList.clear();
				}
			}

			if (_mapCtrl.MoveRouteSearch(itr->mapPos, max(0, _status.move - itr->moveCnt - 1), addResultPosListVec2, _team))
			{
				coverCheck = false;
				_moveDirList.emplace_front(MoveInf(itr->dir, itr->attack, itr->mapPos));
				auto addResultPos = *addResultPosListVec2[itr->mapPos.y][itr->mapPos.x].begin();
				_moveDirList.emplace_back(MoveInf(addResultPos.dir, addResultPos.attack, addResultPos.mapPos));

				for (auto resutlPos = itr->prev; resutlPos->prev != nullptr; resutlPos = resutlPos->prev)
				{
					_moveDirList.emplace_front(MoveInf(resutlPos->dir, resutlPos->attack, resutlPos->mapPos));
				}
				break;
			}
		}
	}
}

Charactor::Charactor(const uint8_t level, const Vector2Int& mapPos, const Team team, const unsigned int groupNum, MapCtrl& mapCtrl, SceneController& ctrl,
	std::vector<std::shared_ptr<Effect>>& effects, Camera& camera)
	: _team(team), _groupNum(groupNum), _mapCtrl(mapCtrl), _controller(ctrl), _effects(effects), Actor(camera)
{
	_pos = (mapPos * _mapCtrl.GetChipSize().ToVector2Int()).ToVector2();
	_isMoveAnim = false;

	_moveSpeed = 4;
	_isSelect = false;
	_canMove = true;
	_moveActive = false;
	_isDying = false;

	_animator = make_shared<Animator>();

	_status = Status();

	_dyingAnimAlphaTL = make_unique<Track<float>>();
	_dyingAnimAlphaTL->AddKey(0, 1.0f);
	_dyingAnimAlphaTL->AddKey(60, 0.0f);

	_updater = &Charactor::NormalUpdate;
	_drawer = &Charactor::NormalDraw;

	_attackRange = Range(2,2);

	_battleStartEffect = make_shared<CorsorTarget>(Vector2Int(), _camera, true, _mapCtrl.GetChipSize());

	_rigid = 0;
	_moveStandby = false;

	auto mapSize = _mapCtrl.GetMapSize();
	_resultPosListVec2.resize(mapSize.h);
	for (auto& resultPosListVec : _resultPosListVec2)
	{
		resultPosListVec.resize(mapSize.w);
	}
}

Charactor::~Charactor()
{
}

void Charactor::Update(const Input& input)
{
	(this->*_updater)(input);
}

void Charactor::Draw()
{
	(this->*_drawer)();
}

void Charactor::InitAnim()
{
	const Size divSize = Size(32, 32);

	int cnt = 0;
	auto nextRectCenterOffset = [&](std::vector<Rect>& animRectVec, int cnt)
	{
		for (auto& rect : animRectVec)
		{
			rect.center.y += divSize.h;
		}
	};

	std::vector<Rect> animRectVec;
	animRectVec.emplace_back(Rect(Vector2Int(16, 16), divSize));
	animRectVec.emplace_back(Rect(Vector2Int(16 + divSize.w * 2, 16), divSize));

	_animator->AddAnim("DownWalk", animRectVec, 30, true);
	nextRectCenterOffset(animRectVec, ++cnt);
	_animator->AddAnim("LeftWalk", animRectVec, 30, true);
	nextRectCenterOffset(animRectVec, ++cnt);
	_animator->AddAnim("RightWalk", animRectVec, 30, true);
	nextRectCenterOffset(animRectVec, ++cnt);
	_animator->AddAnim("UpWalk", animRectVec, 30, true);

	_animator->ChangeAnim("LeftWalk");

	_dirTable[Dir::left]	= DirInf(Vector2Int(-1, 0), "LeftWalk", 270.0f * DX_PI / 180.0f);
	_dirTable[Dir::right]	= DirInf(Vector2Int(1, 0), "RightWalk", 90.0f * DX_PI / 180.0f);
	_dirTable[Dir::up]		= DirInf(Vector2Int(0, -1), "UpWalk",	0.0f * DX_PI / 180.0f);
	_dirTable[Dir::down]	= DirInf(Vector2Int(0, 1), "DownWalk",	180.0f *  DX_PI / 180.0f);

	_dir = Dir::down;
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
	// 同じマスに移動 || 移動アニメーション中
	if (mapPos == GetMapPos() || _isMoveAnim)return false;

	_moveDirList.clear();

	auto oneLineResutlList = CreateResultPosList(mapPos);
	if (oneLineResutlList.size() <= 0)return false;

	CreateMoveDirList(oneLineResutlList);

	//// _resultPosListからmoveDirListを作るために
	//Astar::ResultPos* rp = startResultPos.prev;

	//// attackマスが何マス続くかの数(攻撃範囲内かの確認のため)
	//int attackMassCnt = 0;
	//for (; rp->prev != nullptr;)
	//{
	//	oneLineResutlList.emplace_back(*rp);
	//	if (rp->attack)
	//	{
	//		attackMassCnt++;
	//	}
	//	rp = rp->prev;
	//}

	//// 攻撃範囲外のマスを削除
	//for (int i = _attackRange.max; i < attackMassCnt; i++)
	//{
	//	oneLineResutlList.pop_front();
	//}

	

	//if (_moveDirList.size() > 0)
	//{
	//	auto rItr = _moveDirList.rbegin();
	//	rItr++;
	//	for (;rItr != _moveDirList.rend(); rItr++)
	//	{
	//		rItr->attack = false;
	//	}
	//}

	_isMoveAnim = true;

	_status.move = max(_status.move - oneLineResutlList.begin()->moveCnt, 0);

	return false;
}

void Charactor::CharactorDataInit(const CharactorType& type, const uint8_t& level)
{
	_charactorType = type;

	auto charactorData = Application::Instance().GetDataBase().GetCharactorData(_charactorType);
	_name = charactorData.name;
	_animator->SetImageHandle(Application::Instance().GetDataBase().GetCharactorImageHandle(_charactorType, _team));

	InitAnim();


	_status = charactorData.initialStatus;
	_status.attribute = Attribute::red;

	_status.health += level * charactorData.statusGrowRate.health / 100.0f;
	_status.power += level * charactorData.statusGrowRate.power / 100.0f;
	_status.defense += level * charactorData.statusGrowRate.defense / 100.0f;
	_status.magic_defense += level * charactorData.statusGrowRate.magic_defense / 100.0f;
	_status.speed += level * charactorData.statusGrowRate.speed / 100.0f;
	_status.skill += level * charactorData.statusGrowRate.skill / 100.0f;

	_startStatus = _status;

	_iconPath = charactorData.iconImagePath;
	_attackRange = charactorData.range;
}