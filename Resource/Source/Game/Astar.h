#pragma once
#include <list>
#include <vector>
#include <array>
#include "../Utility/Geometry.h"
#include "../Utility/Dir.h"
#include "Team.h"

class Astar
{
public:
	struct ResultPos
	{
		bool attack;
		Vector2Int mapPos;
		ResultPos* parent;
		Dir dir;
		int moveCnt;	// �����ړ���

		ResultPos() :attack(false), mapPos(Vector2Int()), parent(nullptr), dir(Dir::left), moveCnt(0){};
		ResultPos(const bool atc, const Vector2Int& mapP, ResultPos* parent, const Dir d, const unsigned int mc)
			:attack(atc), mapPos(mapP), parent(parent), dir(d), moveCnt(mc) {};
	};

	struct MapData
	{
		int moveCost;
		Team team;

		MapData():moveCost(1), team(Team::max){};
		MapData(const int mc, const Team t) :moveCost(mc), team(t) {};
	};

private:

	enum class SearchState
	{
		non,	// ���T��
		search,	// �T��
		attack	// �U��
	};

	struct SearchPos
	{
		Vector2Int mapPos;	//�}�b�v��̍��W
		Vector2Int parentPos;	// �e�̃}�b�v��̍��W
		SearchState state;	// Search��
		int moveCost;	// �����ړ���
		SearchPos();
		SearchPos(const Vector2Int& mapPos, const Vector2Int& parent, const SearchState state, const int moveCnt);
	};

	std::vector<std::vector<SearchPos>> _searchPosVec2Move;
	std::vector<std::vector<SearchPos>> _searchPosVec2Attack;
	std::array<Vector2Int, Dir::max> _dirTable;

	void ResetSerchPosVec2D(const std::vector<std::vector<MapData>>& mapData);

	bool CheckSearchPosVec2Range(const Vector2Int& checkPos)const;

	// �S�Ă̈ړ��\�͈͂�����
	void AllMoveRouteSerch(const Vector2Int& startMapPos, const int move, 
		const std::vector<std::vector<MapData>>& mapData, std::list<Astar::ResultPos>& resutlPosList, const Team team, const bool addThrough, const bool heal);

public:
	Astar();
	~Astar();

	// �J�n�ʒu����I�[�ʒu�܂ł̃}�X�����X�g�Ɋi�[���ĕԂ��B������Ȃ������ꍇ�̓��X�g����ɂ��ĕԂ�
	void RouteSearch(const Vector2Int& startMapPos, const int move, const Range& attackRange, 
		const std::vector<std::vector<MapData>>& mapData, std::list<Astar::ResultPos>& resutlPosList, const Team team, const bool heal);

	// ��̃��[�g��������悤��
	bool MoveRouteSerch(const Vector2Int& startMapPos, const int move, const std::vector<std::vector<MapData>>& mapData, std::list<Astar::ResultPos>& resutlPosList, const Team team);
};

