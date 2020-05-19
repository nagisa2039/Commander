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

void Astar::RouteSearch(const Vector2Int& startMapPos, const int move, const Range& attackRange, 
	const std::vector<std::vector<MapData>>& mapData, std::list<Astar::ResultPos>& resutlPosList, const Team team)
{
	ResetSerchPosVec2D(mapData);
	_searchPosVec2Move[startMapPos.y][startMapPos.x].moveCost = 0;
	_searchPosVec2Move[startMapPos.y][startMapPos.x].state = Astar::SearchState::search;

	// 移動範囲の検索
	AllMoveRouteSerch(startMapPos, move, mapData, resutlPosList, team, true);
	
	// 攻撃範囲のサーチ
	std::list<Astar::ResultPos> attackResultPosList;
	attackResultPosList.clear();

	auto seachIdxList = list<Vector2Int>();
	seachIdxList.clear();
 	for (auto& resutlPos : resutlPosList)
	{
		// 味方のマスへはいけない && 開始位置でない のでcontinue
		if (mapData[resutlPos.mapPos.y][resutlPos.mapPos.x].team == team && resutlPos.mapPos != startMapPos)
		{
			continue;
		}

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
					attackResultPosList.emplace_back(ResultPos(true, checkPos, &resutlPos, static_cast<Dir>(i), resutlPos.moveCnt));
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

	// 攻撃範囲分を_resutlPosListに結合
	for (const auto& attack : attackResultPosList)
	{
		resutlPosList.emplace_back(attack);
	}

	resutlPosList.sort([](const Astar::ResultPos& lv, const Astar::ResultPos& rv)
	{
			return lv.mapPos.x < rv.mapPos.x;
	});

	return;
}

bool Astar::MoveRouteSerch(const Vector2Int& startMapPos, const int move, const std::vector<std::vector<MapData>>& mapData, std::list<Astar::ResultPos>& resutlPosList, const Team team)
{
	ResetSerchPosVec2D(mapData);
	_searchPosVec2Move[startMapPos.y][startMapPos.x].moveCost = 0;
	_searchPosVec2Move[startMapPos.y][startMapPos.x].state = Astar::SearchState::search;

	resutlPosList.clear();

	auto tmpResutlPosList = resutlPosList;
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
					resutlPosList.emplace_front(ResultPos(false, checkPos, &(*parentIt), static_cast<Dir>(i), moveCnt));
					for (auto parent = &*parentIt; parent->parent != nullptr; parent = parent->parent)
					{
						resutlPosList.emplace_front(*parent);
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

void Astar::AllMoveRouteSerch(const Vector2Int& startMapPos, const int move, const std::vector<std::vector<MapData>>& mapData, std::list<Astar::ResultPos>& resutlPosList, const Team team, const bool addThrough)
{
	resutlPosList.clear();
	resutlPosList.emplace_back(ResultPos(false, startMapPos, nullptr, Dir::max, 0));

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

			auto parentIt = resutlPosList.begin();
			for (; parentIt != resutlPosList.end(); parentIt++)
			{
				if (parentIt->mapPos == nowPos)
				{
					break;
				}
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
				resutlPosList.emplace_back(ResultPos(false, checkPos, &(*parentIt), static_cast<Dir>(i), moveCnt));
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
