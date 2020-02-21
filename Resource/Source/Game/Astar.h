#pragma once
#include <list>
#include <vector>
#include <array>
#include "../Utility/Geometry.h"

class Astar
{
public:
	struct ResultPos
	{
		bool attack;
		Vector2Int mapPos;

		ResultPos() :attack(false), mapPos(Vector2Int()) {};
		ResultPos(const bool atc, const Vector2Int mapP):attack(atc), mapPos(mapP) {};
	};

private:

	enum class SearchState
	{
		non,	// ���T��
		Serch,	// �T����
		move	// �T���ς�
	};

	enum Dir
	{
		left,
		right,
		up,
		down,
		max
	};

	struct SearchPos
	{
		Vector2Int mapPos;	//�}�b�v��̍��W
		Vector2Int parentPos;	// �e�̃}�b�v��̍��W
		SearchState state;	// Search��
		int moveCnt;	// �����ړ���
		SearchPos();
		SearchPos(const Vector2Int& mapPos, const Vector2Int& parent, const SearchState state, const int moveCnt);
	};

	

	std::vector<std::vector<SearchPos>> _serchPosVec2;
	std::array<Vector2Int, Dir::max> _dirTable;

	void ResetSerchPosVec2D(const std::vector<std::vector<int>>& mapData);

public:
	Astar();
	~Astar();

	// �J�n�ʒu����I�[�ʒu�܂ł̃}�X�����X�g�Ɋi�[���ĕԂ��B������Ȃ������ꍇ�̓��X�g����ɂ��ĕԂ�
	std::list<Astar::ResultPos> RouteSearch(const Vector2Int& startMapPos, const int move, const std::vector<std::vector<int>>& mapData);


};

