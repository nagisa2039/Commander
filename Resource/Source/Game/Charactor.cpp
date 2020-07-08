#include "Charactor.h"
#include "MapCtrl.h"
#include "Camera.h"
#include "../Utility/DxLibUtility.h"
#include "Animator.h"
#include "../Scene/SceneController.h"
#include "../Scene/BattleScene.h"
#include "Effect/Effect.h"
#include "Effect/TerrainDamageEffect.h"
#include "Effect/TerrainRecoverEffect.h"
#include <DxLib.h>
#include "Application.h"
#include "Effect/FlyText.h"
#include <algorithm>
#include "FileSystem.h"
#include "Effect/CorsorTarget.h"
#include "DataBase.h"
#include "SaveData.h"

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

	_animator->ChangeAnim(_dirTable[static_cast<size_t>(_dir)].animName);
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
	float sizeRate = 0.3f;
	auto iconRect= Rect(_camera.GetCameraOffset() + _pos.ToVector2Int() + chipSize * Vector2(sizeRate / 2.0f, sizeRate / 2.0f), 
		(chipSize * sizeRate).ToSize());
	Application::Instance().GetDataBase().GetWeaponData(_startStatus.weaponId).GetTypeData().DrawWeaponIcon(iconRect);

	// HPBerの描画
	Size hpberSize = (chipSize * Vector2(0.8f, 0.1f)).ToSize();
	Vector2Int hpLeftup		= drawPos + chipSize * Vector2(0.1f, 0.8f);
	Vector2Int hpRightdown	= hpLeftup + hpberSize;
	DrawBox(hpLeftup + Vector2Int::One() * -2, hpRightdown + Vector2Int::One() * +2, 0xffffff);
	DrawBox(hpLeftup + Vector2Int::One() * -1, hpRightdown + Vector2Int::One() * +1, 0xaaaaaa);
	float berLength = max(min(static_cast<float>(_status.health) / static_cast<float>(_startStatus.health), 1.0f), 0.0f);
	DrawBox(hpLeftup, hpLeftup + hpberSize.ToVector2Int() * Vector2(berLength, 1.0f), GetTeamColor());

	//// レベルの描画
	//DrawFormatString(drawPos.x, drawPos.y, 0x000000, "Level.%d", _status.level);
	//// canMoveの描画
	//drawPos.y += 16;
	//DrawFormatString(drawPos.x, drawPos.y, 0x000000, "CanMove "+ _canMove ? "true" : "false");

	//// targetCntの描画
	//drawPos.y += 16;
	//DrawFormatString(drawPos.x, drawPos.y, 0x000000, "InT%d", _targetCnt.x);
	//drawPos.y += 16;
	//DrawFormatString(drawPos.x, drawPos.y, 0x000000, "OutT%d",_targetCnt.y);
	//drawPos.y += 16;
	//DrawFormatString(drawPos.x, drawPos.y, 0x000000, "ollc%d", _onelineListCnt);
}

void Charactor::DyingDraw()
{
	auto offset = _camera.GetCameraOffset();

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(255 * _dyingAnimAlphaTL->GetValue()));
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
			MoveEnd(true);
		}
		return;
	}

	_pos += (_dirTable[static_cast<size_t>(it->dir)].moveVec * static_cast<float>(_moveSpeed)).ToVector2();
	if (_pos.ToVector2Int() % _mapCtrl.GetChipSize().ToVector2Int() == Vector2Int(0, 0))
	{
		_moveDirList.pop_front();
		if (_moveDirList.size() <= 0)
		{
			MoveEnd(true);
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
	int graphH = Application::Instance().GetFileSystem().GetImageHandle("Resource/Image/Battle/movableMass.png");

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
	for (auto& resultPosListVec : _resultPosListVec2)
	{
		for (auto& resultPosList : resultPosListVec)
		{
			if (resultPosList.size() <= 0)continue;

			Vector2Int mapPos = resultPosList.begin()->mapPos;
			Rect box(offset + (mapPos * chipSize.ToVector2Int() + chipSize * 0.5) + -1, chipSize);

			box.DrawRectGraph(Vector2Int(static_cast<int>(!CheckMoveMapPos(mapPos)) * 32, 0), Size(32, 32), graphH);

			Vector2Int leftup = offset + mapPos * chipSize.ToVector2Int();

			DrawFormatString(leftup.x, leftup.y, 0x000000, "%d,%d", mapPos.x, mapPos.y);
			//DrawFormatString(leftup.x, leftup.y+16, 0x000000, "Cost : %d", resultPosList.begin()->moveCnt);
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

const Status& Charactor::GetStartStatus() const
{
	return _startStatus;
}

const Status& Charactor::GetStatus() const
{
	return _status;
}

BattleStatus Charactor::GetBattleStatus() const
{
	const auto& dataBase = Application::Instance().GetDataBase();
	const auto& mapChipData = dataBase.GetMapChipData(_mapCtrl.GetMapData(GetMapPos()).mapChip);
	const auto& weaponData = dataBase.GetWeaponData(_status.weaponId);

	return BattleStatus(_status, weaponData, mapChipData.defense, mapChipData.avoidance);
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

const Range& Charactor::GetAttackRange() const
{
	return Application::Instance().GetDataBase().GetWeaponData(_startStatus.weaponId).range;
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

const int Charactor::GetHurtPoint() const
{
	return _startStatus.health - _status.health;
}

bool Charactor::GetMoved() const
{
	return _status.move != _startStatus.move;
}

CharactorType Charactor::GetCharactorType() const
{
	return _charactorType;
}

void Charactor::InitmapPos(const Vector2Int& mapPos)
{
	_startMapPos = mapPos;
	_pos = _startMapPos.ToVector2() * (_mapCtrl.GetChipSize().ToVector2Int()).ToVector2();
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

void Charactor::InitStatus(const Status& status)
{
	_status = status;
	_startStatus = status;
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

void Charactor::MoveDecision()
{
	_status.move = 0;
	RouteSearch();
}

void Charactor::MoveEnd(const bool canMove, const bool removeCamera)
{
	if (!canMove)
	{
		_status.move = _startStatus.move;
	}
	_canMove = canMove;
	_isMoveAnim = false;
	if (removeCamera)
	{
		_camera.PopTargetActor();
	}

	_mapCtrl.AllCharactorRouteSearch();
}

void Charactor::RouteSearch()
{
	_mapCtrl.RouteSearch(*this);
}

void Charactor::TurnReset()
{
	_canMove = true;
	_startMapPos = GetMapPos();
}

void Charactor::MoveCancel()
{
	_pos = (_mapCtrl.GetChipSize().ToVector2Int() * _startMapPos).ToVector2();
	_status.move = _startStatus.move;
	_mapCtrl.AllCharactorRouteSearch();
}

void Charactor::SearchAndMove()
{
	MoveMapPos(_mapCtrl.SearchMovePos(*this, _targetCnt));
}

std::vector<std::vector<std::list<Astar::ResultPos>>>& Charactor::GetResutlPosListVec2()
{
	return _resultPosListVec2;
}

void Charactor::AddDamage(const int damage)
{
	_status.health = min(max(_status.health - damage, 0), _startStatus.health);
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
			auto arrowH = Application::Instance().GetFileSystem().GetImageHandle("Resource/Image/UI/arrow.png");
			Size arrowSize;
			GetGraphSize(arrowH, arrowSize);
			DrawRotaGraph(startPos, chipSize.w / static_cast<float>(arrowSize.w), _dirTable[dir_idx].angle, arrowH, true);
			begin = false;
		}
	}

}

bool Charactor::StartTerrainEffect()
{
	auto mapChipData = Application::Instance().GetDataBase().GetMapChipData(_mapCtrl.GetMapData(GetMapPos()).mapChip);

	// 効果なし
	if (mapChipData.recovery == 0)return false;

	if (mapChipData.recovery > 0)
	{
		_terrainEffect = make_shared<TerrainRecoverEffect>(GetCenterPos().ToVector2Int(), _camera);
	}
	else if(mapChipData.recovery < 0)
	{
		_terrainEffect = make_shared<TerrainDamageEffect>(GetCenterPos().ToVector2Int(), _camera);
	}
	_status.health = min(_startStatus.health, max(1, _status.health + _startStatus.health * static_cast<uint8_t>(mapChipData.recovery / 100.0f)));
	_effects.emplace_back(_terrainEffect);

	return true;
}

bool Charactor::GetTerrainEffectEnd()
{
	return _terrainEffect == nullptr || _terrainEffect->GetDelete();
}

std::list<Vector2Int> Charactor::GetAttackPosList() const
{
	std::list<Vector2Int> attackPosList;
	attackPosList.clear();

	for (const auto& resultPosListVec : _resultPosListVec2)
	{
		for (const auto& resultPosList : resultPosListVec)
		{
			for (const auto& resultPos : resultPosList)
			{
				if (resultPos.mapPos == GetMapPos() || _mapCtrl.GetMapPosChar(resultPos.mapPos) == nullptr) continue;
				attackPosList.emplace_back(resultPos.mapPos);
			}
		}
	}
	return attackPosList;
}

bool Charactor::AddExp(uint8_t exp, const uint8_t expMax)
{
	if (_status.exp + exp >= expMax)
	{
		_status.exp = (_status.exp + exp) % expMax;
		return true;
	}
	_status.exp = (_status.exp + exp) % expMax;
	return false;
}

Status Charactor::GetLevelUpStatus()
{
	// grawRateの格率でステータス上昇が起こるか計算
	auto calPin = [](const uint8_t grawRate)
	{
		return grawRate > (rand() % 100) ? 1 : 0;
	};

	Status status;
	auto growRate = Application::Instance().GetDataBase().GetCharactorData(_charactorType).statusGrowRate;
	status.level			= 1;
	status.health			= calPin(growRate.health);
	status.power			= calPin(growRate.power);
	status.magic_power		= calPin(growRate.magic_power);
	status.defense			= calPin(growRate.defense);
	status.magic_defense	= calPin(growRate.magic_defense);
	status.skill			= calPin(growRate.skill);
	status.speed			= calPin(growRate.speed);
	status.luck				= calPin(growRate.luck);

	return status;
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
		if (GetBattleStatus().CheckHeal())
		{
			for (const auto& targetPos : _resultPosListVec2[mapPos.y][mapPos.x])
			{
				// ひとつ前のマスが自分か、空きスペースなら
				auto prevCharactor = _mapCtrl.GetMapPosChar(targetPos.prev->mapPos);
				if (prevCharactor == this || prevCharactor == nullptr)
				{
					startResultPos = targetPos;
					break;
				}
			}
		}
		else
		{
			// 相手の攻撃範囲
			auto targetRange = targetCharactor->GetAttackRange();
			// 一方的に攻撃できる距離
			Range criticalRange = GetAttackRange().GetCriticalRange(targetRange);
			// 一方的に攻撃できる距離があるか
			if (criticalRange != Range(0, 0))
			{
				for (const auto& targetPos : _resultPosListVec2[mapPos.y][mapPos.x])
				{
					// ひとつ前のマスが自分か、空きスペースなら
					if (targetPos.prev == nullptr) continue;

					auto prevCharactor = _mapCtrl.GetMapPosChar(targetPos.prev->mapPos);
					if (prevCharactor == this || prevCharactor == nullptr)
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
				for (const auto& targetPos : _resultPosListVec2[mapPos.y][mapPos.x])
				{
					// ひとつ前のマスが自分か、空きスペースなら
					auto prevCharactor = _mapCtrl.GetMapPosChar(targetPos.prev->mapPos);
					if (prevCharactor == this || prevCharactor == nullptr)
					{
						startResultPos = targetPos;
						break;
					}
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

	// 攻撃開始地点に敵が居ないかを確認するためのフラグ
	bool coverCheck = true;
	bool reSearch = false;
	for (auto itr = resultPosList.begin(); itr != resultPosList.end(); itr++)
	{
		// 移動力を超えるマスは無視する
		if (itr->moveCnt > _status.move) continue;

		if (!coverCheck)
		{
			_moveDirList.emplace_front(MoveInf(itr->dir, itr->attack, itr->mapPos));
			continue;
		}

		if (itr->attack)
		{
			// 現在いるマスでもなければ攻撃開始地点にキャラクターがいる
			if (itr->prev->mapPos != GetMapPos() && _mapCtrl.GetMapPosChar(itr->prev->mapPos) != nullptr) continue;

			coverCheck = false;
			_moveDirList.emplace_front(MoveInf(itr->dir, itr->attack, itr->mapPos));
			continue;
		}

		// 移動先にキャラクターがいないなら
		if (_mapCtrl.GetMapPosChar(itr->mapPos) == nullptr)
		{
			coverCheck = false;
			_moveDirList.emplace_front(MoveInf(itr->dir, itr->attack, itr->mapPos));
			continue;
		}

		// ひとつ前に空きがあるなら採用する
		if (itr->prev != nullptr && _mapCtrl.GetMapPosChar(itr->prev->mapPos) == nullptr)
		{
			coverCheck = false;
			continue;
		}

		// 移動先にほかのキャラクターがいるのでルートの再検索を行う
		Size mapSize = _mapCtrl.GetMapSize();
		list<Astar::ResultPos> addResultPosList;
		addResultPosList.clear();

		list<Astar::ResultPos> excludeList;
		excludeList.clear(); 
		auto exAddItr = itr;
		exAddItr++;
		for (; exAddItr != resultPosList.end(); exAddItr++)
		{
			excludeList.emplace_back(*exAddItr);
		}

		if (reSearch) continue;

		reSearch = true;
		if (_mapCtrl.MoveRouteSearch(itr->mapPos, max(0, min(itr->moveCnt - 1, _status.move - itr->moveCnt - 1)), addResultPosList, _team, excludeList))
		{
			for (const auto& addResultPos : addResultPosList)
			{
				_moveDirList.emplace_front(MoveInf(addResultPos.dir, addResultPos.attack, addResultPos.mapPos));
			}
			_moveDirList.emplace_front(MoveInf(itr->dir, itr->attack, itr->mapPos));

			coverCheck = false;
			continue;
		}
	}

	if (_moveDirList.size() <= 0) return;

	// 最後の要素以外のattackをfalseにする
	auto rItr = _moveDirList.rbegin();
	rItr++;
	for (; rItr != _moveDirList.rend(); rItr++)
	{
		rItr->attack = false;
	}
}

Charactor::Charactor(const uint8_t level, const Vector2Int& mapPos, const Team team, const unsigned int groupNum, MapCtrl& mapCtrl, SceneController& ctrl,
	std::vector<std::shared_ptr<Effect>>& effects, Camera& camera)
	: _team(team), _groupNum(groupNum), _mapCtrl(mapCtrl), _controller(ctrl), _effects(effects), Actor(camera)
{
	InitmapPos(mapPos);
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

	_dir = Dir::down;

	_battleStartEffect = make_shared<CorsorTarget>(Vector2Int(), _camera, true, _mapCtrl.GetChipSize());
	_terrainEffect = make_shared<FlyText>("damage", Vector2Int(), 120, _camera, true);
	_terrainEffect->SetDelete(true);

	_rigid = 0;
	_moveStandby = false;

	auto mapSize = _mapCtrl.GetMapSize();
	_resultPosListVec2.resize(mapSize.h);
	for (auto& resultPosListVec : _resultPosListVec2)
	{
		resultPosListVec.resize(mapSize.w);
	}

	_targetCnt = Vector2Int(0, 0);
	_onelineListCnt = 0;
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

	_animator->ChangeAnim("DownWalk");

	_dirTable[static_cast<size_t>(Dir::left)]	= DirInf(Vector2Int(-1, 0), "LeftWalk",		static_cast<float>(270.0f * DX_PI / 180.0f));
	_dirTable[static_cast<size_t>(Dir::right)]	= DirInf(Vector2Int(1, 0),	"RightWalk",	static_cast<float>(90.0f * DX_PI / 180.0f));
	_dirTable[static_cast<size_t>(Dir::up)]		= DirInf(Vector2Int(0, -1), "UpWalk",		static_cast<float>(0.0f * DX_PI / 180.0f));
	_dirTable[static_cast<size_t>(Dir::down)]	= DirInf(Vector2Int(0, 1),	"DownWalk",		static_cast<float>(180.0f *  DX_PI / 180.0f));

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
	_moveDirList.clear();
	if (!_mapCtrl.CheckMapDataRange(mapPos))
	{
		MoveEnd(false);
		return false;
	}

	// 同じマスに移動 || 移動アニメーション中
	if (mapPos == GetMapPos() || _isMoveAnim)return false;


	auto oneLineResutlList = CreateResultPosList(mapPos);
	_onelineListCnt = static_cast<int>(oneLineResutlList.size());
	if (oneLineResutlList.size() <= 0)return false;

	CreateMoveDirList(oneLineResutlList);

	_isMoveAnim = _moveDirList.size() > 0;

	_status.move = /*max(_status.move - oneLineResutlList.begin()->moveCnt, 0);*/0;

	return false;
}

void Charactor::CharactorDataInit(const CharactorType& type, const uint8_t& level)
{
	_charactorType = type;

	auto dataBase = Application::Instance().GetDataBase();

	auto charactorData = dataBase.GetCharactorData(_charactorType);
	_name = charactorData.name;
	_animator->SetImageHandle(dataBase.GetCharactorImageHandle(_charactorType, _team));

	InitAnim();

	_status = dataBase.GetLevelInitStatus(level, type);
	_startStatus = _status;

	_iconPath = charactorData.iconImagePath;
}