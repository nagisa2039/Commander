#include "Astar.h"
#include <map>
#include <algorithm>

using namespace std;

void Astar::ResetSerchPosVec2D(const std::vector<std::vector<MapData>>& mapData)
{
	_searchPosVec2Move.resize(mapData.size());
	_searchPosVec2Attack.resize(mapData.size());
	for (int i = 0; i < mapData.size(); i++)
	{
		_searchPosVec2Move[i].resize(mapData[0].size());
		_searchPosVec2Attack[i].resize(mapData[0].size());
		for (int j = 0; j < mapData[0].size(); j++)
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
	_dirTable[Dir::left]		= Vector2Int(-1,  0);
	_dirTable[Dir::right]	= Vector2Int( 1,  0);
	_dirTable[Dir::up]		= Vector2Int( 0, -1);
	_dirTable[Dir::down]		= Vector2Int( 0,  1);
}

Astar::~Astar()
{
}

void Astar::RouteSearch(const Vector2Int& startMapPos, const int move, const Range& attackRange, const std::vector<std::vector<MapData>>& mapData, 
	std::vector<std::vector<std::list<Astar::ResultPos>>>& resutlPosListVec2, const Team team, const bool heal)
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

	// 移動範囲の検索
	AllMoveRouteSerch(startMapPos, move, mapData, resutlPosListVec2, team, true, heal);
	
	// 攻撃範囲のサーチ
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
				// 味方の場所からは攻撃範囲を検索しない
				if(startMapPos != resutlPos.mapPos && team == mapData[resutlPos.mapPos.y][resutlPos.mapPos.x].team)continue;

				// 初期化
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
					// 開始地点から四方向のサーチを行う
					for (int i = 0; i < Dir::max; i++)
					{
						// 範囲外チェック
						auto checkPos = *it + _dirTable[i];
						if (!CheckSearchPosVec2Range(checkPos))
						{
							continue;
						}

						// 移動済みか初期マスには移動しない
						if (_searchPosVec2Attack[checkPos.y][checkPos.x].state != Astar::SearchState::non)
						{
							continue;
						}

						_searchPosVec2Attack[checkPos.y][checkPos.x].state = Astar::SearchState::search;

						// 攻撃範囲内かの確認
						auto len = (checkPos - (resutlPos.mapPos));
						int ran = abs(len.x) + abs(len.y);
						if (ran >= attackRange.min && _searchPosVec2Move[checkPos.y][checkPos.x].state == Astar::SearchState::non)
						{
							auto checkTeam = mapData[checkPos.y][checkPos.x].team;
							if (heal)
							{
								if (checkTeam == team || checkTeam == Team::max)
								{
									attackResultPosList.emplace_back(ResultPos(true, checkPos, &resutlPos, static_cast<Dir>(i), resutlPos.moveCnt));
									resutlPos.next = &*attackResultPosList.rbegin();
								}
							}
							else
							{
								if (checkTeam != team)
								{
									attackResultPosList.emplace_back(ResultPos(true, checkPos, &resutlPos, static_cast<Dir>(i), resutlPos.moveCnt));
									resutlPos.next = &*attackResultPosList.rbegin();
								}
							}
						}

						// 最大範囲未満ならそこからさらにSearchする
						if (ran < attackRange.max)
						{
							seachIdxList.emplace_back(checkPos);
						}
					}

					it = seachIdxList.erase(it);
				}

			}

			if (heal)
			{
				for (auto& resutlPos : resutlPosList)
				{
					if (mapData[resutlPos.mapPos.y][resutlPos.mapPos.x].team != team || resutlPos.mapPos == startMapPos)
					{
						continue;
					}

					for (const auto& attack : attackResultPosList)
					{
						if (resutlPos.mapPos == attack.mapPos)
						{
							resutlPos.attack = true;
							continue;
						}
					}

				}
			}
		}
	}

	// 攻撃範囲分を_resutlPosListに結合
	for (const auto& attackResultPos : attackResultPosList)
	{
		resutlPosListVec2[attackResultPos.mapPos.y][attackResultPos.mapPos.x].emplace_back(attackResultPos);
	}

	// moveCntの昇順で並び替え
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

bool Astar::MoveRouteSerch(const Vector2Int& startMapPos, const int move, const std::vector<std::vector<MapData>>& mapData, std::vector<std::vector<std::list<Astar::ResultPos>>>& resutlPosListVec2, const Team team)
{
	ResetSerchPosVec2D(mapData);
	_searchPosVec2Move[startMapPos.y][startMapPos.x].moveCost = 0;
	_searchPosVec2Move[startMapPos.y][startMapPos.x].state = Astar::SearchState::search;

	auto tmpResutlPosList = list<ResultPos>();
	tmpResutlPosList.emplace_back(ResultPos(false, startMapPos, nullptr, Dir::max, 0));

	auto seachIdxList = list<Vector2Int>();
	seachIdxList.clear();

	seachIdxList.emplace_front(startMapPos);

	for (auto it = seachIdxList.begin(); it != seachIdxList.end();)
	{
		Vector2Int nowPos = *it;

		// 開始地点から四方向のサーチを行う
		for (int i = 0; i < Dir::max; i++)
		{
			// 範囲外チェック
			auto checkPos = nowPos + _dirTable[i];
			if (!CheckSearchPosVec2Range(checkPos))
			{
				continue;
			}

			// 移動済みには移動しない
			if (_searchPosVec2Move[checkPos.y][checkPos.x].state != Astar::SearchState::non)
			{
				continue;
			}

			auto parentIt = tmpResutlPosList.begin();
			for (; parentIt != tmpResutlPosList.end(); parentIt++)
			{
				if (parentIt->mapPos == nowPos)
				{
					break;
				}
			}
			
			auto checkPosTeam = mapData[checkPos.y][checkPos.x].team;
			// 移動不可	敵キャラクターがいる
			if (mapData[checkPos.y][checkPos.x].moveCost < 0 || (checkPosTeam != team && checkPosTeam != Team::max))
			{
				continue;
			}

			auto moveCnt = _searchPosVec2Move[nowPos.y][nowPos.x].moveCost + mapData[checkPos.y][checkPos.x].moveCost;

			// 移動可能な距離か
			if (moveCnt <= move)
			{
				tmpResutlPosList.emplace_back(ResultPos(false, checkPos, &(*parentIt), static_cast<Dir>(i), moveCnt));
				// キャラクターがいない
				if (checkPosTeam == Team::max)
				{
					resutlPosListVec2[checkPos.y][checkPos.x].emplace_front(ResultPos(false, checkPos, &(*parentIt), static_cast<Dir>(i), moveCnt));
					for (auto parent = &*parentIt; parent->prev != nullptr; parent = parent->prev)
					{
						resutlPosListVec2[checkPos.y][checkPos.x].emplace_front(*parent);
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
	std::vector<std::vector<std::list<Astar::ResultPos>>>& resutlPosListVec2, const Team team, const bool addThrough, const bool heal)
{
	for (auto& resutlPosListVec : resutlPosListVec2)
	{
		for (auto& resutlPosList : resutlPosListVec)
		{
			resutlPosList.clear();
		}
	}

	resutlPosListVec2[startMapPos.y][startMapPos.x].emplace_back(ResultPos(false, startMapPos, nullptr, Dir::max, 0));

	auto seachIdxList = list<Vector2Int>();
	seachIdxList.clear();

	seachIdxList.emplace_front(startMapPos);

	for (auto it = seachIdxList.begin(); it != seachIdxList.end();)
	{
		Vector2Int nowPos = *it;

		// 開始地点から四方向のサーチを行う
		for (int i = 0; i < Dir::max; i++)
		{
			// 範囲外チェック
			auto checkPos = nowPos + _dirTable[i];
			if (!CheckSearchPosVec2Range(checkPos))
			{
				continue;
			}

			// 移動済みには移動しない
			if (_searchPosVec2Move[checkPos.y][checkPos.x].state != Astar::SearchState::non)
			{
				continue;
			}


			// 移動不可
			if (mapData[checkPos.y][checkPos.x].moveCost < 0 || (mapData[checkPos.y][checkPos.x].team != team && mapData[checkPos.y][checkPos.x].team != Team::max))
			{
				continue;
			}

			auto moveCnt = _searchPosVec2Move[nowPos.y][nowPos.x].moveCost + mapData[checkPos.y][checkPos.x].moveCost;

			// 移動可能な距離か
			if (moveCnt <= move)
			{
				_searchPosVec2Move[checkPos.y][checkPos.x] = SearchPos(checkPos, nowPos, Astar::SearchState::search, moveCnt);
				seachIdxList.emplace_back(checkPos);
				if (resutlPosListVec2[nowPos.y][nowPos.x].size() > 0)
				{
					auto prev = &*resutlPosListVec2[nowPos.y][nowPos.x].begin();
					resutlPosListVec2[checkPos.y][checkPos.x].emplace_back(ResultPos(false, checkPos, prev, static_cast<Dir>(i), moveCnt));
					prev->next = &*resutlPosListVec2[checkPos.y][checkPos.x].rbegin();
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
