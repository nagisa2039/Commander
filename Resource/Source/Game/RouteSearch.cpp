#include "RouteSearch.h"
#include <map>
#include <algorithm>
#include <windows.h>

using namespace std;
using namespace SearchData;

namespace
{
	constexpr int SEARCH_LIMIT_TIME = 100;
}

void RouteSearch::ResetSerchPosVec2D(const SearchMapData& mapData)
{
	auto mapDataSizeY = mapData.size();
	auto mapDataSizeX = mapData[0].size();
	_searchPosVec2Move.resize(mapDataSizeY);
	_searchPosVec2Attack.resize(mapDataSizeY);
	for (int i = 0; i < mapDataSizeY; ++i)
	{
		_searchPosVec2Move[i].resize(mapDataSizeX);
		_searchPosVec2Attack[i].resize(mapDataSizeX);
		for (int j = 0; j < mapDataSizeX; ++j)
		{
			_searchPosVec2Move[i][j].state = RouteSearch::SearchState::non;
			_searchPosVec2Attack[i][j].state = RouteSearch::SearchState::non;
		}
	}
}

bool RouteSearch::CheckSearchPosVec2Range(const Vector2Int& checkPos) const
{
	return (checkPos.x >= 0 && checkPos.x < static_cast<int>(_searchPosVec2Move[0].size())
		&& checkPos.y >= 0 && checkPos.y < static_cast<int>(_searchPosVec2Move.size()));
}

RouteSearch::RouteSearch()
{
	_dirTable[static_cast<size_t>(Dir::left)]	= Vector2Int{ -1,  0 };
	_dirTable[static_cast<size_t>(Dir::right)]	= Vector2Int{ 1,  0 };
	_dirTable[static_cast<size_t>(Dir::up)]		= Vector2Int{ 0, -1 };
	_dirTable[static_cast<size_t>(Dir::down)]	= Vector2Int{ 0,  1 };
}

RouteSearch::~RouteSearch()
{
}

void RouteSearch::Search(const Vector2Int& startMapPos, const int move, const Range& attackRange, const SearchMapData& mapData,
	RouteSearchData& resutlPosListVec2, const Team team, const bool heal, const bool searchEnemy)
{
	ResetSerchPosVec2D(mapData);
	_searchPosVec2Move[startMapPos.y][startMapPos.x].moveCost = 0;
	_searchPosVec2Move[startMapPos.y][startMapPos.x].state = RouteSearch::SearchState::search;

	for (auto& resutlPosListVec : resutlPosListVec2)
	{
		for (auto& resutlPosList : resutlPosListVec)
		{
			resutlPosList.clear();
		}
	}

	// 移動範囲の検索
	AllMoveRouteSerch(startMapPos, move, mapData, resutlPosListVec2, team, heal, searchEnemy);
	
	// 攻撃範囲のサーチ
	ResultList attackResultPosList;
	attackResultPosList.clear();

	ResetSerchPosVec2D(mapData);
	_searchPosVec2Move[startMapPos.y][startMapPos.x].moveCost = 0;
	_searchPosVec2Move[startMapPos.y][startMapPos.x].state = RouteSearch::SearchState::search;
	auto seachIdxList = list<Vector2Int>();
	seachIdxList.clear();

	for (auto& resutlPosListVec : resutlPosListVec2)
	{
		for (auto& resutlPosList : resutlPosListVec)
		{
			for (auto& resutlPos : resutlPosList)
			{
				// 移動範囲内で味方の場所からは攻撃範囲を検索しない
				if(!searchEnemy && startMapPos != resutlPos.mapPos && team == mapData[resutlPos.mapPos.y][resutlPos.mapPos.x].team)continue;

				// 初期化
				seachIdxList.clear();
				seachIdxList.emplace_front(resutlPos.mapPos);
				for (auto& searchPosVec : _searchPosVec2Attack)
				{
					for (auto& searchPos : searchPosVec)
					{
						searchPos.state = RouteSearch::SearchState::non;
					}
				}
				_searchPosVec2Attack[resutlPos.mapPos.y][resutlPos.mapPos.x].state = RouteSearch::SearchState::search;

				for (auto it = seachIdxList.begin(); it != seachIdxList.end();)
				{
					// 開始地点から四方向のサーチを行う
					for (int i = 0; i < static_cast<int>(Dir::max); i++)
					{
						// 範囲外チェック
						auto checkPos = *it + _dirTable[i];
						if (!CheckSearchPosVec2Range(checkPos))
						{
							continue;
						}

						// 移動済みか初期マスには移動しない
						if (_searchPosVec2Attack[checkPos.y][checkPos.x].state != RouteSearch::SearchState::non)
						{
							continue;
						}

						_searchPosVec2Attack[checkPos.y][checkPos.x].state = RouteSearch::SearchState::search;

						// 攻撃範囲内かの確認
						auto len = (checkPos - (resutlPos.mapPos));
						int ran = abs(len.x) + abs(len.y);
						if (ran >= attackRange.min && _searchPosVec2Move[checkPos.y][checkPos.x].state == RouteSearch::SearchState::non)
						{
							auto checkTeam = mapData[checkPos.y][checkPos.x].team;
							if (heal)
							{
								if (checkTeam == Team::max || (mapData[checkPos.y][checkPos.x].isHurt && checkTeam == team))
								{
									attackResultPosList.emplace_back(ResultPos{ true, checkPos, &resutlPos, static_cast<Dir>(i), resutlPos.moveCnt});
								}
							}
							else
							{
								if (checkTeam != team)
								{
									attackResultPosList.emplace_back(ResultPos{ true, checkPos, &resutlPos, static_cast<Dir>(i), resutlPos.moveCnt});
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
			resutlPosList.sort([](const ResultPos& left, const ResultPos& right)
			{
				return left.moveCnt < right.moveCnt;
			});
		}
	}

	return;
}

bool RouteSearch::MoveRouteSerch(const Vector2Int& startMapPos, const int move, const SearchMapData& mapData,
	ResultList& resutlPosList, const Team team, const ResultList& excludeList)
{
	ResetSerchPosVec2D(mapData);
	_searchPosVec2Move[startMapPos.y][startMapPos.x].moveCost = 0;
	_searchPosVec2Move[startMapPos.y][startMapPos.x].state = RouteSearch::SearchState::search;

	for (const auto& exclude : excludeList)
	{
		_searchPosVec2Move[exclude.mapPos.y][exclude.mapPos.x].state = RouteSearch::SearchState::search;
	}

	resutlPosList.clear();

	ResultList tmpList;
	tmpList.clear();
	tmpList.emplace_back(ResultPos{ false, startMapPos, nullptr, Dir::max, 0 });


	auto seachIdxList = list<Vector2Int>();
	seachIdxList.clear();

	seachIdxList.emplace_front(startMapPos);

	int dirMax = static_cast<int>(Dir::max);
	auto startTime = GetTickCount64();
	for (auto it = seachIdxList.begin(); it != seachIdxList.end();)
	{
		if (GetTickCount64() - startTime > SEARCH_LIMIT_TIME)
		{
			assert(false);
			break;
		}
		Vector2Int nowPos = *it;

		// 開始地点から四方向のサーチを行う
		for (int i = 0; i < dirMax; ++i)
		{
			// 範囲外チェック
			auto checkPos = nowPos + _dirTable[i];
			if (!CheckSearchPosVec2Range(checkPos))
			{
				continue;
			}

			// 移動済みには移動しない
			if (CheckMoved(checkPos, startMapPos, nowPos))
			{
				continue;
			}

			// 親のイテレータを取得
			auto parentIt = tmpList.begin();
			for (; parentIt != tmpList.end(); parentIt++)
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
				tmpList.emplace_back(ResultPos{ false, checkPos, &(*parentIt), static_cast<Dir>(i), moveCnt });
				// キャラクターがいない
				if (checkPosTeam == Team::max)
				{
					for (auto tmpResult = *tmpList.rbegin(); tmpResult.mapPos != startMapPos; tmpResult = *tmpResult.prev)
					{
						resutlPosList.emplace_back(tmpResult);
					}
					return true;
				}
				_searchPosVec2Move[checkPos.y][checkPos.x] = SearchPos{ checkPos, nowPos, RouteSearch::SearchState::search, moveCnt };
				seachIdxList.emplace_back(checkPos);
			}
		}

		it = seachIdxList.erase(it);
	}
	return false;
}

void RouteSearch::AllMoveRouteSerch(const Vector2Int& startMapPos, const int move, const SearchMapData& mapData,
	RouteSearchData& resutlSearchData, const Team team, const bool heal, const bool searchEnemy)
{
	resutlSearchData[startMapPos.y][startMapPos.x].emplace_back(ResultPos{ false, startMapPos, nullptr, Dir::max, 0 });

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
	_searchPosVec2Move[startMapPos.y][startMapPos.x] = SearchPos{ startMapPos, startMapPos, RouteSearch::SearchState::search, 0 };

	int dirMax = static_cast<int>(Dir::max);
	auto startTime = GetTickCount64();
	for (auto it = seachIdxList.begin(); it != seachIdxList.end();)
	{
		// タイムアウトブレイク
		if (GetTickCount64() - startTime > SEARCH_LIMIT_TIME)
		{
			assert(false);
			break;
		}

		Vector2Int nowPos = *it;

		// 開始地点から四方向のサーチを行う
		for (int i = 0; i < dirMax; ++i)
		{
			// 範囲外チェック
			auto checkPos = nowPos + _dirTable[i];
			if (!CheckSearchPosVec2Range(checkPos))
			{
				continue;
			}

			// 移動不可
			if (mapData[checkPos.y][checkPos.x].moveCost < 0 || (mapData[checkPos.y][checkPos.x].team != team && mapData[checkPos.y][checkPos.x].team != Team::max))
			{
				continue;
			}

			auto moveCnt = _searchPosVec2Move[nowPos.y][nowPos.x].moveCost + mapData[checkPos.y][checkPos.x].moveCost;
			if (moveCnt >= _searchPosVec2Move[checkPos.y][checkPos.x].moveCost)
			{
				continue;
			}

			// 移動可能な距離か
			if (moveCnt <= (searchEnemy ? 50 : move))
			{
				_searchPosVec2Move[checkPos.y][checkPos.x] = SearchPos{ checkPos, nowPos, RouteSearch::SearchState::search, moveCnt };
				seachIdxList.emplace_back(checkPos);
				auto& nowResultPosList = resutlSearchData[nowPos.y][nowPos.x];
				if (nowResultPosList.size() > 0)
				{
					auto it = find_if(nowResultPosList.begin(), nowResultPosList.end(), 
						[moveCnt](const ResultPos& rp) {return rp.moveCnt < moveCnt; });
					ResultPos* prev = (it == nowResultPosList.end() ? nullptr : &*it);
					resutlSearchData[checkPos.y][checkPos.x].emplace_back(ResultPos{ false, checkPos, prev, static_cast<Dir>(i), moveCnt });
				}
				else
				{
					resutlSearchData[checkPos.y][checkPos.x].emplace_back(ResultPos{ false, checkPos, nullptr, static_cast<Dir>(i), moveCnt });
				}
			}
		}

		it = seachIdxList.erase(it);
	}
}

bool RouteSearch::CheckMoved(const Vector2Int& checkPos, const Vector2Int& startMapPos, const Vector2Int& nowPos)
{
	if (startMapPos == nowPos)return false;
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