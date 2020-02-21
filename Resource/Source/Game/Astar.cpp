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
	_dirTable[left]		= Vector2Int(-1,  0);
	_dirTable[right]	= Vector2Int( 1,  0);
	_dirTable[up]		= Vector2Int( 0, -1);
	_dirTable[down]		= Vector2Int( 0,  1);
}

Astar::~Astar()
{
}

std::list<Astar::ResultPos> Astar::RouteSearch(const Vector2Int & startMapPos, const int move, const std::vector<std::vector<int>>& mapData)
{
	auto result = list<Astar::ResultPos>();
	result.clear();

	ResetSerchPosVec2D(mapData);

	auto seachIdxList = list<Vector2Int>();
	seachIdxList.clear();

	seachIdxList.emplace_front(startMapPos);

	_serchPosVec2[startMapPos.y][startMapPos.x].moveCnt = 0;
	for (auto it = seachIdxList.begin(); it != seachIdxList.end();)
	{
		auto idx = *it;
		Vector2Int nowPos = _serchPosVec2[idx.y][idx.x].mapPos;

		// �J�n�n�_����l�����̃T�[�`���s��
		for (int j = 0; j < Dir::max; j++)
		{
			// �͈͊O�`�F�b�N
			auto checkPos = nowPos + _dirTable[j];
			if (checkPos.x < 0 || checkPos.x >= static_cast<int>(_serchPosVec2[0].size())
				|| checkPos.y < 0 || checkPos.y >= static_cast<int>(_serchPosVec2.size()))
			{
				continue;
			}

			// �ړ��ς݂��ړ��s�̏ꏊ�ɂ͈ړ����Ȃ�
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

			if (moveCnt > move)
			{
				result.emplace_back(ResultPos(true, checkPos));
			}
			else
			{
				seachIdxList.emplace_back(checkPos);
			}
		}

		result.emplace_back(ResultPos(false, *it));
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
