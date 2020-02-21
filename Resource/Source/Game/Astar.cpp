#include "Astar.h"
#include "Dir.h"
#include "MapMng.h"

using namespace std;

const Astar::SearchPos Astar::GetSearchPos(const Vector2Int& mapPos)const
{
	return _serchPosVec[_mapMng.ChangeMapPosToIdx(mapPos)];
}

void Astar::SetSearchPos(const Vector2Int & nowPos, const int fromtStart, const Vector2Int & endMapPos, const Vector2Int & parent, const SearchState state)
{
	int fromEnd = abs(endMapPos.x - nowPos.x) + abs(endMapPos.y - nowPos.y);
	auto tmp = SearchPos(nowPos, fromtStart, fromEnd, parent, state);
	_serchPosVec[_mapMng.ChangeMapPosToIdx(nowPos)] = tmp;
}

Astar::Astar(const MapMng & mapMng): _mapMng(mapMng)
{
	_serchPosVec.resize(_mapMng.GetMapSize().w * _mapMng.GetMapSize().h);
	for (auto& serchPos : _serchPosVec)
	{
		serchPos.state = Astar::SearchState::non;
	}
}

Astar::~Astar()
{
}

std::list<Vector2Int> Astar::RouteSearch(const Vector2Int & startPos, const Vector2Int & endPos)
{
	auto chipSize = _mapMng.GetChipSize();
	auto startMapPos = Vector2Int(startPos.x / chipSize.w, startPos.y / chipSize.h);
	auto endMapPos = Vector2Int(endPos.x / chipSize.w, endPos.y / chipSize.h);

	auto result = list<Vector2Int>();
	result.clear();

	if (!_mapMng.CheckMovableBlock(_mapMng.GetBlockType(endMapPos, false))
	 || !_mapMng.CheckMovableBlock(_mapMng.GetBlockType(startMapPos, false)))
	{
		return result;
	}


	if (endMapPos == startMapPos)
	{
		return result;
	}

	for (auto& serchPos : _serchPosVec)
	{
		serchPos.state = Astar::SearchState::non;
	}
	auto searchList = list<SearchPos>();
	searchList.clear();

	Vector2Int nowPos = startMapPos;
	auto& dirInfoTbl = _mapMng.GetDirInfoTbl();

	auto CreateResult = [&]()
	{
		result.emplace_front(endMapPos);
		while (true)
		{
			auto idx = _mapMng.ChangeMapPosToIdx(*result.begin());
			if (_serchPosVec[idx].parentPos == startMapPos)
			{
				return result;
			}
			result.emplace_front(_serchPosVec[idx].parentPos);
		}
	};

	int moveCnt = 0;

	int cnt = 0;
	while (++cnt < 100)
	{
		// �J�n�n�_����l�����̃T�[�`���s��
		for (int j = 0; j < Dir::dir_max; j++)
		{
			// �͈͊O�`�F�b�N
			auto checkPos = nowPos + dirInfoTbl[j].move;
			auto idx = _mapMng.ChangeMapPosToIdx(checkPos);
			if (idx < 0 || idx >= static_cast<int>(_serchPosVec.size()))
			{
				continue;
			}

			// �ړ��ς݂��ړ��s�̏ꏊ�ɂ͈ړ����Ȃ�
			auto block = _mapMng.GetBlockType(checkPos, false);
			if (_serchPosVec[idx].state != Astar::SearchState::non
				|| checkPos == startMapPos
				|| !_mapMng.CheckMovableBlock(block)
				|| _mapMng.CheckFire(checkPos,false))
			{
					continue;
			}
			SetSearchPos(checkPos, moveCnt + 1, endMapPos, nowPos, Astar::SearchState::Serch);
			searchList.emplace_front(GetSearchPos(checkPos));

			// �ړ��n�_���G���h�Ȃ�I��
			if (checkPos == endMapPos)
			{
				return CreateResult();
			}
		}

		auto itr = searchList.begin();

		// �T�[�`���ʂ���ړ�����
		auto search = searchList.begin();
		if (search == searchList.end())
		{
			return result;
		}
		search++;
		for (; search != searchList.end(); search++)
		{
			if (itr->fromEnd + itr->fromStart > search->fromEnd + search->fromStart)
			{
				itr = search;
			}
			else if (itr->fromEnd + itr->fromStart == search->fromEnd + search->fromStart)
			{
				// �����ꍇ�X�^�[�g�n�_����̋������傫�������̗p����
				if (itr->fromStart < search->fromStart)
				{
					itr = search;
				}
			}
		}
		_serchPosVec[_mapMng.ChangeMapPosToIdx((*itr).mapPos)].state = SearchState::move;
		nowPos = (*itr).mapPos;
		moveCnt = (*itr).fromStart;
		searchList.erase(itr);
	}

	return result;
}

Astar::SearchPos::SearchPos()
{
}

Astar::SearchPos::SearchPos(const Vector2Int & mapPos, const int start, const int end, const Vector2Int & parent, const SearchState state)
{
	this->mapPos = mapPos;
	this->parentPos = parent;
	this->fromStart = start;
	this->fromEnd = end;
	this->state = state;
}
