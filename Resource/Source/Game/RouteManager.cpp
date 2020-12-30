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
				// �ړ��}�X���Ȃ����
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
	// �ړ��\�ȃ��[�g������
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
				// �ЂƂO�̃}�X���������A�󂫃X�y�[�X�Ȃ�
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
			// ����̍U���͈�
			auto targetRange = targetCharactor->GetAttackRange();
			// ����I�ɍU���ł��鋗��
			Range criticalRange = _charactor.GetAttackRange().GetCriticalRange(targetRange);
			// ����I�ɍU���ł��鋗�������邩
			if (criticalRange != Range(0, 0))
			{
				for (const auto& targetPos : _resultPosListVec2[mapPos.y][mapPos.x])
				{
					// �ЂƂO�̃}�X���������A�󂫃X�y�[�X�Ȃ�
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
					// �ЂƂO�̃}�X���������A�󂫃X�y�[�X�Ȃ�
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
		// �G�����Ȃ��}�X�ւ̈ړ��Ȃ̂ňړ��}�X��D��I�ɍ̗p����
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

	// �U���J�n�n�_�ɓG�����Ȃ������m�F���邽�߂̃t���O
	bool coverCheck = true;
	bool reSearch = false;
	auto& status = _charactor.GetStatus();
	auto team = _charactor.GetTeam();
	auto mapPos = _charactor.GetMapPos();

	for (auto itr = resultPosList.begin(); itr != resultPosList.end(); itr++)
	{
		// �ړ��͂𒴂���}�X�͖�������
		if (itr->moveCnt > status.move) continue;

		if (!coverCheck)
		{
			_moveDirList.emplace_front(MoveInf{ itr->dir, itr->attack, itr->mapPos });
			continue;
		}

		if (itr->attack)
		{
			// ���݂���}�X�ł��Ȃ���΍U���J�n�n�_�ɃL�����N�^�[������
			if (itr->prev->mapPos != mapPos && _mapCtrl.GetMapPosChar(itr->prev->mapPos) != nullptr) continue;

			coverCheck = false;
			_moveDirList.emplace_front(MoveInf{ itr->dir, itr->attack, itr->mapPos });
			continue;
		}

		// �ړ���ɃL�����N�^�[�����Ȃ��Ȃ�
		if (_mapCtrl.GetMapPosChar(itr->mapPos) == nullptr)
		{
			coverCheck = false;
			_moveDirList.emplace_front(MoveInf{ itr->dir, itr->attack, itr->mapPos });
			continue;
		}

		// �ЂƂO�ɋ󂫂�����Ȃ�̗p����
		if (itr->prev != nullptr && _mapCtrl.GetMapPosChar(itr->prev->mapPos) == nullptr)
		{
			coverCheck = false;
			continue;
		}

		// �ړ���ɂق��̃L�����N�^�[������̂Ń��[�g�̍Č������s��
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

	// �Ō�̗v�f�ȊO��attack��false�ɂ���
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
				// �U���}�X�ȊO�͏��O
				if (!resultPos.attack)continue;

				// �L�����N�^�[�̂��Ȃ��}�X�͏��O
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
	// �͈͓��̓G���i�[���郊�X�g
	list<TargetCharactor> targetCharactorList;

	auto addTargetCharactor = [this, &battleStatus](const std::list<Astar::ResultPos> resultPosList,
		list<TargetCharactor>& targetCharactorList, bool in)
	{
		for (const auto& resultPos : resultPosList)
		{
			// �U���}�X�ɂȂ�܂�continue
			if (!resultPos.attack)
			{
				continue;
			}

			auto mapCharactor = _mapCtrl.GetMapPosChar(resultPos.mapPos);
			// ���̃}�X�ɃL�����N�^�[�����邩
			if (mapCharactor == nullptr)continue;

			// �T���Ă���`�[�������邩
			if (battleStatus.CheckHeal() != (_charactor.GetTeam() == mapCharactor->GetTeam()))continue;

			auto mapPosSub = mapCharactor->GetMapPos() - (resultPos.prev == nullptr ? _charactor.GetMapPos() : resultPos.prev->mapPos);
			int distance = abs(mapPosSub.x) + abs(mapPosSub.y);

			// �U���J�n�n�_�ɂ���L�����N�^�[
			auto prevChar = _mapCtrl.GetMapPosChar(resultPos.prev->mapPos);
			// �U���͈͓���
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
			// �ł��_���[�W���󂯂Ă���L�����N�^�[��T��
			targetCharactorList.sort([](const TargetCharactor& left, const TargetCharactor& right)
				{
					return left.charactor->GetHurtPoint() > right.charactor->GetHurtPoint();
				});
			return targetCharactorList.begin()->charactor->GetMapPos();
		}
	}

	// �I��
	for (const auto& targetCharactor : targetCharactorList)
	{
		// �G�̍U���͈͊O����̍U�����H
		if (!targetCharactor.charactor->GetAttackRange().Hit(targetCharactor.distance)
			|| targetCharactor.charactor->GetBattleStatus().CheckHeal())
		{
			return targetCharactor.charactor->GetMapPos();
		}
	}

	// �G�̍U���͈͊O����U���ł��Ȃ��̂ōŏ��Ɍ������G�̏ꏊ�Ɍ�����
	if (targetCharactorList.size() > 0)
	{
		// �ł��_���[�W��^������L�����N�^�[��T��
		targetCharactorList.sort([&battleStatus](const TargetCharactor& left, const TargetCharactor& right)
			{
				return battleStatus.GetDamage(left.charactor->GetBattleStatus()) > battleStatus.GetDamage(right.charactor->GetBattleStatus());
			});
		return targetCharactorList.begin()->charactor->GetMapPos();
	}

	//----------------------------------------------------------------------------------------------------------------------
	// �͈͓��ɓG�����Ȃ��̂Ŕ͈͊O����T��
	_mapCtrl.GetAstar().RouteSearch(_charactor.GetMapPos(), battleStatus.status.move, battleStatus.weaponData.range,
		mapVec2, resultPosListVec2, _charactor.GetTeam(), battleStatus.CheckHeal(), _charactor.GetMoveActive());

	// �h�ȊO�̓G���i�[���郊�X�g
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
			// ��ԋ߂��L�����N�^�[���̗p����
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
		// ��ԋ߂��L�����N�^�[���̗p����
		outRangeCharactorList.sort([](const TargetCharactor& lval, const TargetCharactor& rval)
			{
				return lval.resultPos.moveCnt < rval.resultPos.moveCnt;
			});
		auto targetCharactor = *outRangeCharactorList.begin();
		return targetCharactor.charactor->GetMapPos();
	}
	return Vector2Int(-1, -1);
}