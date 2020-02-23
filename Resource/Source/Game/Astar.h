#pragma once
#include <list>
#include <vector>
#include <array>
#include "../Utility/Geometry.h"
#include "../Utility/Dir.h"

class Astar
{
public:
	struct ResultPos
	{
		bool attack;
		Vector2Int mapPos;
		ResultPos* parent;
		Dir dir;

		ResultPos() :attack(false), mapPos(Vector2Int()), parent(nullptr), dir(left){};
		ResultPos(const bool atc, const Vector2Int& mapP, ResultPos* parent, const Dir d)
			:attack(atc), mapPos(mapP), parent(parent), dir(d) {};
	};

private:

	enum class SearchState
	{
		non,	// ���T��
		Serch,	// �T����
		move	// �T���ς�
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

