#include "Astar.h"

using namespace std;

void Astar::ResetSerchPosVec2D(const std::vector<std::vector<int>>& mapData)
{
	_serchPosVec2.resize(mapData.size());
	for (auto& serchPosVec : _serchPosVec2)
	{
		serchPosVec.resize(mapData[0].size());
		for (auto& serchPos : serchPosVec)
		{
			serchPos.state = Astar::SearchState::non;
		}
	}
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

std::list<Astar::ResultPos> Astar::RouteSearch(const Vector2Int & startMapPos, const int move, const std::vector<std::vector<int>>& mapData)
{
	auto result = list<Astar::ResultPos>();
	result.clear();
	result.emplace_back(ResultPos(false, startMapPos, nullptr, Dir::max));

	ResetSerchPosVec2D(mapData);

	auto seachIdxList = list<Vector2Int>();
	seachIdxList.clear();

	seachIdxList.emplace_front(startMapPos);

	_serchPosVec2[startMapPos.y][startMapPos.x].moveCnt = 0;
	for (auto it = seachIdxList.begin(); it != seachIdxList.end();)
	{
		Vector2Int nowPos = *it;

		// 開始地点から四方向のサーチを行う
		for (int j = 0; j < Dir::max; j++)
		{
			// 範囲外チェック
			auto checkPos = nowPos + _dirTable[j];
			if (checkPos.x < 0 || checkPos.x >= static_cast<int>(_serchPosVec2[0].size())
				|| checkPos.y < 0 || checkPos.y >= static_cast<int>(_serchPosVec2.size()))
			{
				continue;
			}

			// 移動済みか移動不可の場所には移動しない
			if (_serchPosVec2[checkPos.y][checkPos.x].state != Astar::SearchState::non
				|| checkPos == startMapPos
				|| mapData[checkPos.y][checkPos.x] == -1)
			{
				continue;
			}

			_serchPosVec2[checkPos.y][checkPos.x]
				= SearchPos(checkPos, nowPos, Astar::SearchState::Serch, 
					_serchPosVec2[nowPos.y][nowPos.x].moveCnt + mapData[checkPos.y][checkPos.x]);
			auto moveCnt = _serchPosVec2[checkPos.y][checkPos.x].moveCnt;
			
			auto parentIt = result.begin();
			for(; parentIt != result.end(); parentIt++)
			{
				if (parentIt->mapPos == nowPos)
				{
					break;
				}
			}

			if (moveCnt > move)
			{
				result.emplace_back(ResultPos(true, checkPos, &(*parentIt), static_cast<Dir>(j)));
			}
			else
			{
				seachIdxList.emplace_back(checkPos);
				result.emplace_back(ResultPos(false, checkPos, &(*parentIt), static_cast<Dir>(j)));
			}
		}

		it = seachIdxList.erase(it);
	}

	return result;
}

Astar::SearchPos::SearchPos()
{
	this->mapPos = Vector2Int();
	this->parentPos = Vector2Int();
	this->state = Astar::SearchState::non;
	this->moveCnt = 0;
}

Astar::SearchPos::SearchPos(const Vector2Int & mapPos, const Vector2Int & parent, const SearchState state, const int moveCnt)
{
	this->mapPos = mapPos;
	this->parentPos = parent;
	this->state = state;
	this->moveCnt = moveCnt;
}
