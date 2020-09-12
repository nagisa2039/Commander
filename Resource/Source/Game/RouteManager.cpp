#include "RouteManager.h"
#include "MapCtrl.h"
#include "Camera.h"
#include "Application.h"
#include "FileSystem.h"
#include "Charactor.h"
#include <DxLib.h>
#include "DxLibUtility.h"
#include <algorithm>

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
	bool heal = Application::Instance().GetDataBase().GetWeaponData(_charactor.GetStartStatus().weaponId).GetTypeData().heal;

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
			attackStartPos = itr->prev->mapPos;
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
			_moveDirList.emplace_front(MoveInf(itr->dir, itr->attack, itr->mapPos));
			continue;
		}

		if (itr->attack)
		{
			// ���݂���}�X�ł��Ȃ���΍U���J�n�n�_�ɃL�����N�^�[������
			if (itr->prev->mapPos != mapPos && _mapCtrl.GetMapPosChar(itr->prev->mapPos) != nullptr) continue;

			coverCheck = false;
			_moveDirList.emplace_front(MoveInf(itr->dir, itr->attack, itr->mapPos));
			continue;
		}

		// �ړ���ɃL�����N�^�[�����Ȃ��Ȃ�
		if (_mapCtrl.GetMapPosChar(itr->mapPos) == nullptr)
		{
			coverCheck = false;
			_moveDirList.emplace_front(MoveInf(itr->dir, itr->attack, itr->mapPos));
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
		if (_mapCtrl.MoveRouteSearch(itr->mapPos, max(0, min(itr->moveCnt - 1, status.move - itr->moveCnt - 1)), addResultPosList, team, excludeList))
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

	// �Ō�̗v�f�ȊO��attack��false�ɂ���
	auto rItr = _moveDirList.rbegin();
	rItr++;
	for (; rItr != _moveDirList.rend(); rItr++)
	{
		rItr->attack = false;
	}
}