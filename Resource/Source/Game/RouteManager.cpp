#include "RouteManager.h"
#include "MapCtrl.h"
#include "Camera.h"
#include "Application.h"
#include "FileSystem.h"
#include "Charactor.h"
#include <DxLib.h>
#include "DxLibUtility.h"
#include <algorithm>
#include "Astar.h"

using namespace std;

RouteManager::RouteManager(Charactor& charactor, MapCtrl& mapCtrl, Camera& camera)
	:_charactor(charactor), _mapCtrl(mapCtrl), _camera(camera)
{
	auto& mapSize = _mapCtrl.GetMapSize();
	_resultPosListVec2.resize(mapSize.h);
	for (auto& resultPosListVec : _resultPosListVec2)
	{
		resultPosListVec.resize(mapSize.w);
	}
}

RouteManager::~RouteManager()
{
}

void RouteManager::DrawMovableMass(const uint8_t alpha) const
{
	auto offset = _camera.GetCameraOffset();
	auto chipSize = _mapCtrl.GetChipSize();
	int graphH = ImageHandle("Resource/Image/Battle/movableMass.png");
	bool heal = DataBase::Instance().GetWeaponData(_charactor.GetStartStatus().weaponId).GetTypeData().heal;

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
	for (auto& resultPosListVec : _resultPosListVec2)
	{
		for (auto& resultPosList : resultPosListVec)
		{
			if (resultPosList.size() <= 0)continue;

			Vector2Int mapPos = resultPosList.begin()->mapPos;
			Rect box(offset + (mapPos * chipSize.ToVector2Int() + chipSize * 0.5) + -1, chipSize);

			const int HEAL_SCR_X = 64;
			const int ATTACK_SCR_X = 32;
			int scrX = 0;
			auto charactor = _mapCtrl.GetMapPosChar(mapPos);
			auto mapPosMassCnt = CheckMapPos(mapPos);

			if (heal && mapPosMassCnt.y > 0 && charactor && charactor->GetHurtPoint() > 0)
			{
				scrX = HEAL_SCR_X;
			}
			else
			{
				// 移動マスがなければ
				if (mapPosMassCnt.x <= 0)
				{
					scrX = heal ? HEAL_SCR_X : ATTACK_SCR_X;
				}
			}
			box.DrawRectGraph(Vector2Int(scrX, 0), Size(32, 32), graphH);
		}
	}

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
}

Vector2Int RouteManager::CheckMapPos(const Vector2Int& mapPos) const
{
	Vector2Int ret(0, 0);
	for (const auto& resutlPos : _resultPosListVec2[mapPos.y][mapPos.x])
	{
		resutlPos.attack ? ret.y++ : ret.x++;
	}
	return ret;
}

bool RouteManager::GetAttackStartPos(Vector2Int& attackStartPos, const Vector2Int& targetMapPos) const
{
	const auto targetChar = _mapCtrl.GetMapPosChar(targetMapPos);
	if (!targetChar)return false;

	auto& resultPosList = _resultPosListVec2[targetMapPos.y][targetMapPos.x];
	if (resultPosList.size() <= 0)return false;

	bool set = false;
	auto targetAttackRange = targetChar->GetAttackRange();
	auto itr = resultPosList.begin();
	for (; itr != resultPosList.end(); itr++)
	{
		if (!itr->attack)continue;

		set = true;
		attackStartPos = itr->prev->mapPos;

		auto sub = targetMapPos - itr->prev->mapPos;
		int attackDistance = abs(sub.x) + abs(sub.y);
		if (!targetAttackRange.Hit(attackDistance))
		{
			return true;
		}
	}

	return set;
}

std::vector<std::vector<std::list<Astar::ResultPos>>>& RouteManager::GetResutlPosListVec2()
{
	return _resultPosListVec2;
}

std::list<RouteManager::MoveInf>& RouteManager::GetMoveDirList()
{
	return _moveDirList;
}

void RouteManager::DrawRoute(const Vector2Int& targetPos)
{
	auto mapPos = _charactor.GetMapPos();
	auto& dirTable = _charactor.GetDirTable();
	if (targetPos == mapPos) return;
	if (_resultPosListVec2[targetPos.y][targetPos.x].size() <= 0)return;

	bool begin = true;
	for (const auto& route : CreateResultPosList(targetPos))
	{
		if (begin && route.attack)continue;

		auto offset = _camera.GetCameraOffset();
		auto chipSize = _mapCtrl.GetChipSize();
		size_t dir_idx = static_cast<size_t>(route.dir);

		if (0 > dir_idx || dir_idx >= static_cast<int>(Dir::max))return;

		int lineWidth = 10;
		unsigned int color = 0xffff00;
		Vector2Int startPos = offset + route.mapPos * chipSize + chipSize * 0.5f;
		Vector2Int endPos;
		if (route.prev == nullptr)
		{
			endPos = offset + mapPos * chipSize + chipSize * 0.5f;
			DrawCircle(endPos, lineWidth / 2, color);
		}
		else
		{
			endPos = offset + route.prev->mapPos * chipSize + chipSize * 0.5f;
			if (route.prev->dir != route.dir)
			{
				DrawCircle(endPos, lineWidth / 2, color);
			}
		}
		DrawLine(startPos, endPos, color, lineWidth);
		if (begin)
		{
			auto arrowH = ImageHandle("Resource/Image/UI/arrow.png");
			Size arrowSize;
			GetGraphSize(arrowH, arrowSize);
			DrawRotaGraph(startPos, chipSize.w / static_cast<float>(arrowSize.w), dirTable[dir_idx].angle, arrowH, true);
			begin = false;
		}
	}

}

std::list<Vector2Int> RouteManager::GetAttackPosList() const
{
	std::list<Vector2Int> attackPosList;
	attackPosList.clear();
	auto mapPos = _charactor.GetMapPos();

	for (const auto& resultPosListVec : _resultPosListVec2)
	{
		for (const auto& resultPosList : resultPosListVec)
		{
			for (const auto& resultPos : resultPosList)
			{
				if (resultPos.mapPos == mapPos || _mapCtrl.GetMapPosChar(resultPos.mapPos) == nullptr) continue;
				attackPosList.emplace_back(resultPos.mapPos);
			}
		}
	}
	return attackPosList;
}

std::list<Astar::ResultPos> RouteManager::CreateResultPosList(const Vector2Int& mapPos) const
{
	std::list<Astar::ResultPos> routeList;
	routeList.clear();
	// 移動可能なルートが無い
	if (_resultPosListVec2[mapPos.y][mapPos.x].size() <= 0)return routeList;

	auto selfMapPos = _charactor.GetMapPos();

	Astar::ResultPos startResultPos = *_resultPosListVec2[mapPos.y][mapPos.x].begin();
	auto targetCharactor = _mapCtrl.GetMapPosChar(mapPos);
	if (targetCharactor != nullptr)
	{
		if (_charactor.GetBattleStatus().CheckHeal())
		{
			for (const auto& targetPos : _resultPosListVec2[mapPos.y][mapPos.x])
			{
				// ひとつ前のマスが自分か、空きスペースなら
				auto prevCharactor = _mapCtrl.GetMapPosChar(targetPos.prev->mapPos);
				if (prevCharactor == &_charactor || prevCharactor == nullptr)
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
			Range criticalRange = _charactor.GetAttackRange().GetCriticalRange(targetRange);
			// 一方的に攻撃できる距離があるか
			if (criticalRange != Range(0, 0))
			{
				for (const auto& targetPos : _resultPosListVec2[mapPos.y][mapPos.x])
				{
					// ひとつ前のマスが自分か、空きスペースなら
					auto prevCharactor = _mapCtrl.GetMapPosChar(targetPos.prev->mapPos);
					if (prevCharactor == &_charactor || prevCharactor == nullptr)
					{
						Vector2Int startPos = targetPos.prev == nullptr ? selfMapPos : targetPos.prev->mapPos;
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
					if (prevCharactor == &_charactor || prevCharactor == nullptr)
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

void RouteManager::CreateMoveDirList(const std::list<Astar::ResultPos>& resultPosList)
{
	_moveDirList.clear();

	// 攻撃開始地点に敵が居ないかを確認するためのフラグ
	bool coverCheck = true;
	bool reSearch = false;
	auto& status = _charactor.GetStatus();
	auto team = _charactor.GetTeam();
	auto mapPos = _charactor.GetMapPos();

	for (auto itr = resultPosList.begin(); itr != resultPosList.end(); itr++)
	{
		// 移動力を超えるマスは無視する
		if (itr->moveCnt > status.move) continue;

		if (!coverCheck)
		{
			_moveDirList.emplace_front(MoveInf{ itr->dir, itr->attack, itr->mapPos });
			continue;
		}

		if (itr->attack)
		{
			// 現在いるマスでもなければ攻撃開始地点にキャラクターがいる
			if (itr->prev->mapPos != mapPos && _mapCtrl.GetMapPosChar(itr->prev->mapPos) != nullptr) continue;

			coverCheck = false;
			_moveDirList.emplace_front(MoveInf{ itr->dir, itr->attack, itr->mapPos });
			continue;
		}

		// 移動先にキャラクターがいないなら
		if (_mapCtrl.GetMapPosChar(itr->mapPos) == nullptr)
		{
			coverCheck = false;
			_moveDirList.emplace_front(MoveInf{ itr->dir, itr->attack, itr->mapPos });
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
		if (MoveRouteSearch(itr->mapPos, max(0, min(itr->moveCnt - 1, status.move - itr->moveCnt - 1)), addResultPosList, team, excludeList))
		{
			for (const auto& addResultPos : addResultPosList)
			{
				_moveDirList.emplace_front(MoveInf{ addResultPos.dir, addResultPos.attack, addResultPos.mapPos });
			}
			_moveDirList.emplace_front(MoveInf{itr->dir, itr->attack, itr->mapPos
		});

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

void RouteManager::SearchRangeTarget()
{
	_inRangeCharactorList.clear();
	for (const auto& resultPosListVec : _resultPosListVec2)
	{
		for (const auto& resultPosList : resultPosListVec)
		{
			for (const auto& resultPos : resultPosList)
			{
				// 攻撃マス以外は除外
				if (!resultPos.attack)continue;

				// キャラクターのいないマスは除外
				auto charactor = _mapCtrl.GetMapPosChar(resultPos.mapPos);
				if (!charactor)continue;

				_inRangeCharactorList.emplace_back(charactor);
			}
		}
	}
}

bool RouteManager::CheckInRageTarget()
{
	return _inRangeCharactorList.size() > 0;
}

void RouteManager::RouteSearch()
{
	std::vector<std::vector<Astar::MapData>> mapVec2;
	_mapCtrl.CreateMapVec(mapVec2, _charactor.GetTeam());

	_mapCtrl.GetAstar().RouteSearch(_charactor.GetMapPos(), _charactor.GetStatus().move, _charactor.GetAttackRange(),
		mapVec2, _charactor.GetRouteManager()->GetResutlPosListVec2(), _charactor.GetTeam(),
		DataBase::Instance().GetWeaponTypeDataFromWeaponId(_charactor.GetStatus().weaponId).heal);

	SearchRangeTarget();

	return;
}

bool RouteManager::MoveRouteSearch(const Vector2Int& startPos, const unsigned int move, std::list<Astar::ResultPos>& resutlPosList, const Team team, const std::list<Astar::ResultPos>& excludeList)
{
	if (move <= 0)return false;

	std::vector<std::vector<Astar::MapData>> mapVec2;
	_mapCtrl.CreateMapVec(mapVec2, team);

	return _mapCtrl.GetAstar().MoveRouteSerch(startPos, move, mapVec2, resutlPosList, team, excludeList);
}

Vector2Int RouteManager::SearchMovePos()
{
	std::vector<std::vector<Astar::MapData>> mapVec2;
	_mapCtrl.CreateMapVec(mapVec2, _charactor.GetTeam());
	auto battleStatus = _charactor.GetBattleStatus();
	auto& resultPosListVec2 = _charactor.GetRouteManager()->GetResutlPosListVec2();

	struct TargetCharactor
	{
		Charactor* charactor;
		int distance;
		Astar::ResultPos resultPos;

		TargetCharactor() :charactor(nullptr), distance(1), resultPos(Astar::ResultPos()) {};
		TargetCharactor(Charactor* ch, const int di, const Astar::ResultPos& rp) :charactor(ch), distance(di), resultPos(rp) {};
	};
	// 範囲内の敵を格納するリスト
	list<TargetCharactor> targetCharactorList;

	auto addTargetCharactor = [this, &battleStatus](const std::list<Astar::ResultPos> resultPosList,
		list<TargetCharactor>& targetCharactorList, bool in)
	{
		for (const auto& resultPos : resultPosList)
		{
			// 攻撃マスになるまでcontinue
			if (!resultPos.attack)
			{
				continue;
			}

			auto mapCharactor = _mapCtrl.GetMapPosChar(resultPos.mapPos);
			// そのマスにキャラクターがいるか
			if (mapCharactor == nullptr)continue;

			// 探しているチームがいるか
			if (battleStatus.CheckHeal() != (_charactor.GetTeam() == mapCharactor->GetTeam()))continue;

			auto mapPosSub = mapCharactor->GetMapPos() - (resultPos.prev == nullptr ? _charactor.GetMapPos() : resultPos.prev->mapPos);
			int distance = abs(mapPosSub.x) + abs(mapPosSub.y);

			// 攻撃開始地点にいるキャラクター
			auto prevChar = _mapCtrl.GetMapPosChar(resultPos.prev->mapPos);
			// 攻撃範囲内か
			bool inRange = resultPos.moveCnt <= battleStatus.status.move && (prevChar == nullptr || prevChar == &_charactor);
			if (inRange&& in)
			{
				targetCharactorList.emplace_back(TargetCharactor(mapCharactor, distance, resultPos));
				continue;
			}
			if (!inRange && !in)
			{
				targetCharactorList.emplace_back(TargetCharactor(mapCharactor, distance, resultPos));
				continue;
			}

		}
	};

	for(auto& charactor : _inRangeCharactorList)
	{
		auto mapPos = charactor->GetMapPos();
		addTargetCharactor(resultPosListVec2[mapPos.y][mapPos.x], targetCharactorList, true);
	}

	if (battleStatus.CheckHeal())
	{
		if (targetCharactorList.size() > 0)
		{
			// 最もダメージを受けているキャラクターを探す
			targetCharactorList.sort([](const TargetCharactor& left, const TargetCharactor& right)
				{
					return left.charactor->GetHurtPoint() > right.charactor->GetHurtPoint();
				});
			return targetCharactorList.begin()->charactor->GetMapPos();
		}
	}

	// 選別
	for (const auto& targetCharactor : targetCharactorList)
	{
		// 敵の攻撃範囲外からの攻撃か？
		if (!targetCharactor.charactor->GetAttackRange().Hit(targetCharactor.distance)
			|| targetCharactor.charactor->GetBattleStatus().CheckHeal())
		{
			return targetCharactor.charactor->GetMapPos();
		}
	}

	// 敵の攻撃範囲外から攻撃できないので最初に見つけた敵の場所に向かう
	if (targetCharactorList.size() > 0)
	{
		// 最もダメージを与えられるキャラクターを探す
		targetCharactorList.sort([&battleStatus](const TargetCharactor& left, const TargetCharactor& right)
			{
				return battleStatus.GetDamage(left.charactor->GetBattleStatus()) > battleStatus.GetDamage(right.charactor->GetBattleStatus());
			});
		return targetCharactorList.begin()->charactor->GetMapPos();
	}

	//----------------------------------------------------------------------------------------------------------------------
	// 範囲内に敵がいないので範囲外から探す
	_mapCtrl.GetAstar().RouteSearch(_charactor.GetMapPos(), battleStatus.status.move, battleStatus.weaponData.range,
		mapVec2, resultPosListVec2, _charactor.GetTeam(), battleStatus.CheckHeal(), _charactor.GetMoveActive());

	// 派以外の敵を格納するリスト
	list<TargetCharactor> outRangeCharactorList;
	
	for (const auto& resultPosListVec : resultPosListVec2)
	{
		for (const auto& resultPosList : resultPosListVec)
		{
			addTargetCharactor(resultPosList, outRangeCharactorList, false);
		}
	}

	if (battleStatus.CheckHeal())
	{
		if (outRangeCharactorList.size() > 0)
		{
			// 一番近いキャラクターを採用する
			outRangeCharactorList.sort([](const TargetCharactor& lval, const TargetCharactor& rval)
				{
					return lval.resultPos.moveCnt < rval.resultPos.moveCnt;
				});
			for (auto& targetCharactor : outRangeCharactorList)
			{
				if (targetCharactor.charactor->GetHurtPoint() > 0)
				{
					return targetCharactor.charactor->GetMapPos();
				}
			}
		}
		return Vector2Int(-1, -1);
	}

	if (outRangeCharactorList.size() > 0)
	{
		// 一番近いキャラクターを採用する
		outRangeCharactorList.sort([](const TargetCharactor& lval, const TargetCharactor& rval)
			{
				return lval.resultPos.moveCnt < rval.resultPos.moveCnt;
			});
		auto targetCharactor = *outRangeCharactorList.begin();
		return targetCharactor.charactor->GetMapPos();
	}
	return Vector2Int(-1, -1);
}