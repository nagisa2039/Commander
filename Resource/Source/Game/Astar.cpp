#include "Astar.h"
#include <map>
#include <algorithm>

using namespace std;

void Astar::ResetSerchPosVec2D(const std::vector<std::vector<MapData>>& mapData)
{
	int mapDataSizeY = mapData.size();
	int mapDataSizeX = mapData[0].size();
	_searchPosVec2Move.resize(mapDataSizeY);
	_searchPosVec2Attack.resize(mapDataSizeY);
	for (int i = 0; i < mapDataSizeY; ++i)
	{
		_searchPosVec2Move[i].resize(mapDataSizeX);
		_searchPosVec2Attack[i].resize(mapDataSizeX);
		for (int j = 0; j < mapDataSizeX; ++j)
		{
			_searchPosVec2Move[i][j].state = Astar::SearchState::non;
			_searchPosVec2Attack[i][j].state = Astar::SearchState::non;
		}
	}
}

bool Astar::CheckSearchPosVec2Range(const Vector2Int& checkPos) const
{
	return (checkPos.x >= 0 && checkPos.x < static_cast<int>(_searchPosVec2Move[0].size())
		&& checkPos.y >= 0 && checkPos.y < static_cast<int>(_searchPosVec2Move.size()));
}

Astar::Astar()
{
	_dirTable[static_cast<size_t>(Dir::left)]		= Vector2Int(-1,  0);
	_dirTable[static_cast<size_t>(Dir::right)]	= Vector2Int( 1,  0);
	_dirTable[static_cast<size_t>(Dir::up)]		= Vector2Int( 0, -1);
	_dirTable[static_cast<size_t>(Dir::down)]		= Vector2Int( 0,  1);
}

Astar::~Astar()
{
}

void Astar::RouteSearch(const Vector2Int& startMapPos, const int move, const Range& attackRange, const std::vector<std::vector<MapData>>& mapData, 
	std::vector<std::vector<std::list<Astar::ResultPos>>>& resutlPosListVec2, const Team team, const bool heal, const bool searchEnemy)
{
	ResetSerchPosVec2D(mapData);
	_searchPosVec2Move[startMapPos.y][startMapPos.x].moveCost = 0;
	_searchPosVec2Move[startMapPos.y][startMapPos.x].state = Astar::SearchState::search;

	for (auto& resutlPosListVec : resutlPosListVec2)
	{
		for (auto& resutlPosList : resutlPosListVec)
		{
			resutlPosList.clear();
		}
	}

	// �ړ��͈͂̌���
	AllMoveRouteSerch(startMapPos, move, mapData, resutlPosListVec2, team, true, heal, searchEnemy);
	
	// �U���͈͂̃T�[�`
	std::list<Astar::ResultPos> attackResultPosList;
	attackResultPosList.clear();

	ResetSerchPosVec2D(mapData);
	_searchPosVec2Move[startMapPos.y][startMapPos.x].moveCost = 0;
	_searchPosVec2Move[startMapPos.y][startMapPos.x].state = Astar::SearchState::search;
	auto seachIdxList = list<Vector2Int>();
	seachIdxList.clear();

	for (auto& resutlPosListVec : resutlPosListVec2)
	{
		for (auto& resutlPosList : resutlPosListVec)
		{
			for (auto& resutlPos : resutlPosList)
			{
				// �ړ��͈͓��Ŗ����̏ꏊ����͍U���͈͂��������Ȃ�
				if(!searchEnemy && startMapPos != resutlPos.mapPos && team == mapData[resutlPos.mapPos.y][resutlPos.mapPos.x].team)continue;

				// ������
				seachIdxList.clear();
				seachIdxList.emplace_front(resutlPos.mapPos);
				for (auto& searchPosVec : _searchPosVec2Attack)
				{
					for (auto& searchPos : searchPosVec)
					{
						searchPos.state = Astar::SearchState::non;
					}
				}
				_searchPosVec2Attack[resutlPos.mapPos.y][resutlPos.mapPos.x].state = Astar::SearchState::search;

				for (auto it = seachIdxList.begin(); it != seachIdxList.end();)
				{
					// �J�n�n�_����l�����̃T�[�`���s��
					for (int i = 0; i < static_cast<int>(Dir::max); i++)
					{
						// �͈͊O�`�F�b�N
						auto checkPos = *it + _dirTable[i];
						if (!CheckSearchPosVec2Range(checkPos))
						{
							continue;
						}

						// �ړ��ς݂������}�X�ɂ͈ړ����Ȃ�
						if (_searchPosVec2Attack[checkPos.y][checkPos.x].state != Astar::SearchState::non)
						{
							continue;
						}

						_searchPosVec2Attack[checkPos.y][checkPos.x].state = Astar::SearchState::search;

						// �U���͈͓����̊m�F
						auto len = (checkPos - (resutlPos.mapPos));
						int ran = abs(len.x) + abs(len.y);
						if (ran >= attackRange.min && _searchPosVec2Move[checkPos.y][checkPos.x].state == Astar::SearchState::non)
						{
							auto checkTeam = mapData[checkPos.y][checkPos.x].team;
							if (heal)
							{
								if ((checkTeam == team || checkTeam == Team::max)/* && mapData[checkPos.y][checkPos.x].isHurt*/)
								{
									attackResultPosList.emplace_back(ResultPos(true, checkPos, &resutlPos, static_cast<Dir>(i), resutlPos.moveCnt));
								}
							}
							else
							{
								if (checkTeam != team)
								{
									attackResultPosList.emplace_back(ResultPos(true, checkPos, &resutlPos, static_cast<Dir>(i), resutlPos.moveCnt));
								}
							}
						}

						// �ő�͈͖����Ȃ炻�����炳���Search����
						if (ran < attackRange.max)
						{
							seachIdxList.emplace_back(checkPos);
						}
					}

					it = seachIdxList.erase(it);
				}

			}
		}
	}

	// �U���͈͕���_resutlPosList�Ɍ���
	for (const auto& attackResultPos : attackResultPosList)
	{
		resutlPosListVec2[attackResultPos.mapPos.y][attackResultPos.mapPos.x].emplace_back(attackResultPos);
	}

	// moveCnt�̏����ŕ��ёւ�
	for (auto& resutlPosListVec : resutlPosListVec2)
	{
		for (auto& resutlPosList : resutlPosListVec)
		{
			resutlPosList.sort([](const Astar::ResultPos& left, const Astar::ResultPos& right)
			{
				return left.moveCnt < right.moveCnt;
			});
		}
	}

	return;
}

bool Astar::MoveRouteSerch(const Vector2Int& startMapPos, const int move, const std::vector<std::vector<MapData>>& mapData, std::list<Astar::ResultPos>& resutlPosList, const Team team, const std::list<Astar::ResultPos>& excludeList)
{
	ResetSerchPosVec2D(mapData);
	_searchPosVec2Move[startMapPos.y][startMapPos.x].moveCost = 0;
	_searchPosVec2Move[startMapPos.y][startMapPos.x].state = Astar::SearchState::search;

	for (const auto& exclude : excludeList)
	{
		_searchPosVec2Move[exclude.mapPos.y][exclude.mapPos.x].state = Astar::SearchState::search;
	}

	resutlPosList.clear();

	list<Astar::ResultPos> tmpList;
	tmpList.clear();
	tmpList.emplace_back(ResultPos(false, startMapPos, nullptr, Dir::max, 0));


	auto seachIdxList = list<Vector2Int>();
	seachIdxList.clear();

	seachIdxList.emplace_front(startMapPos);

	int dirMax = static_cast<int>(Dir::max);
	for (auto it = seachIdxList.begin(); it != seachIdxList.end();)
	{
		Vector2Int nowPos = *it;

		// �J�n�n�_����l�����̃T�[�`���s��
		for (int i = 0; i < dirMax; ++i)
		{
			// �͈͊O�`�F�b�N
			auto checkPos = nowPos + _dirTable[i];
			if (!CheckSearchPosVec2Range(checkPos))
			{
				continue;
			}

			// �ړ��ς݂ɂ͈ړ����Ȃ�
			if (CheckMoved(checkPos, startMapPos, nowPos))
			{
				continue;
			}

			// �e�̃C�e���[�^���擾
			auto parentIt = tmpList.begin();
			for (; parentIt != tmpList.end(); parentIt++)
			{
				if (parentIt->mapPos == nowPos)
				{
					break;
				}
			}
			
			auto checkPosTeam = mapData[checkPos.y][checkPos.x].team;
			// �ړ��s��	�G�L�����N�^�[������
			if (mapData[checkPos.y][checkPos.x].moveCost < 0 || (checkPosTeam != team && checkPosTeam != Team::max))
			{
				continue;
			}

			auto moveCnt = _searchPosVec2Move[nowPos.y][nowPos.x].moveCost + mapData[checkPos.y][checkPos.x].moveCost;

			// �ړ��\�ȋ�����
			if (moveCnt <= move)
			{
				tmpList.emplace_back(ResultPos(false, checkPos, &(*parentIt), static_cast<Dir>(i), moveCnt));
				// �L�����N�^�[�����Ȃ�
				if (checkPosTeam == Team::max)
				{
					for (auto tmpResult = *tmpList.rbegin(); tmpResult.mapPos != startMapPos; tmpResult = *tmpResult.prev)
					{
						resutlPosList.emplace_back(tmpResult);
					}
					return true;
				}
				_searchPosVec2Move[checkPos.y][checkPos.x] = SearchPos(checkPos, nowPos, Astar::SearchState::search, moveCnt);
				seachIdxList.emplace_back(checkPos);
			}
		}

		it = seachIdxList.erase(it);
	}
	return false;
}

void Astar::AllMoveRouteSerch(const Vector2Int& startMapPos, const int move, const std::vector<std::vector<MapData>>& mapData, 
	std::vector<std::vector<std::list<Astar::ResultPos>>>& resutlPosListVec2, const Team team, const bool addThrough, const bool heal, const bool searchEnemy)
{
	resutlPosListVec2[startMapPos.y][startMapPos.x].emplace_back(ResultPos(false, startMapPos, nullptr, Dir::max, 0));

	auto seachIdxList = list<Vector2Int>();
	seachIdxList.clear();

	seachIdxList.emplace_front(startMapPos);
	for (auto& searchPosVec : _searchPosVec2Move)
	{
		for (auto& searchPos : searchPosVec)
		{
			searchPos.moveCost = 999;
		}
	}
	_searchPosVec2Move[startMapPos.y][startMapPos.x] = SearchPos(startMapPos, startMapPos, Astar::SearchState::search, 0);

	int dirMax = static_cast<int>(Dir::max);
	for (auto it = seachIdxList.begin(); it != seachIdxList.end();)
	{
		Vector2Int nowPos = *it;

		// �J�n�n�_����l�����̃T�[�`���s��
		for (int i = 0; i < dirMax; ++i)
		{
			// �͈͊O�`�F�b�N
			auto checkPos = nowPos + _dirTable[i];
			if (!CheckSearchPosVec2Range(checkPos))
			{
				continue;
			}

			// �ړ��s��
			if (mapData[checkPos.y][checkPos.x].moveCost < 0 || (mapData[checkPos.y][checkPos.x].team != team && mapData[checkPos.y][checkPos.x].team != Team::max))
			{
				continue;
			}

			auto moveCnt = _searchPosVec2Move[nowPos.y][nowPos.x].moveCost + mapData[checkPos.y][checkPos.x].moveCost;
			if (moveCnt >= _searchPosVec2Move[checkPos.y][checkPos.x].moveCost)
			{
				continue;
			}

			// �ړ��\�ȋ�����
			if (moveCnt <= (searchEnemy ? 50 : move))
			{
				_searchPosVec2Move[checkPos.y][checkPos.x] = SearchPos(checkPos, nowPos, Astar::SearchState::search, moveCnt);
				seachIdxList.emplace_back(checkPos);
				auto& nowResultPosList = resutlPosListVec2[nowPos.y][nowPos.x];
				if (nowResultPosList.size() > 0)
				{
					auto it = find_if(nowResultPosList.begin(), nowResultPosList.end(), 
						[moveCnt](const Astar::ResultPos& rp) {return rp.moveCnt < moveCnt; });
					Astar::ResultPos* prev = (it == nowResultPosList.end() ? nullptr : &*it);
					resutlPosListVec2[checkPos.y][checkPos.x].emplace_back(ResultPos(false, checkPos, prev, static_cast<Dir>(i), moveCnt));
				}
				else
				{
					resutlPosListVec2[checkPos.y][checkPos.x].emplace_back(ResultPos(false, checkPos, nullptr, static_cast<Dir>(i), moveCnt));
				}
			}
		}

		it = seachIdxList.erase(it);
	}
}

bool Astar::CheckMoved(const Vector2Int& checkPos, const Vector2Int& startMapPos, const Vector2Int& nowPos)
{
	for (auto parentPos = _searchPosVec2Move[nowPos.y][nowPos.x].parentPos; parentPos != startMapPos;
		parentPos = _searchPosVec2Move[parentPos.y][parentPos.x].parentPos)
	{
		if (checkPos == parentPos)
		{
			return true;
		}
	}
	return false;
}

Astar::SearchPos::SearchPos()
{
	this->mapPos = Vector2Int();
	this->parentPos = Vector2Int();
	this->state = Astar::SearchState::non;
	this->moveCost = 0;
}

Astar::SearchPos::SearchPos(const Vector2Int & mapPos, const Vector2Int & parent, const SearchState state, const int moveCnt)
{
	this->mapPos = mapPos;
	this->parentPos = parent;
	this->state = state;
	this->moveCost = moveCnt;
}
