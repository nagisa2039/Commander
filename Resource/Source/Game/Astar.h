#pragma once
#include <list>
#include <vector>
#include "../Geometry.h"

class MapMng;

class Astar
{
private:
	const MapMng& _mapMng;

	enum class SearchState
	{
		non,	// ���T��
		Serch,	// �T����
		move	// �T���ς�
	};

	struct SearchPos
	{
		Vector2Int mapPos;
		Vector2Int parentPos;	// �e�̃}�X
		int fromStart;	// �X�^�[�g�n�_����̋���
		int fromEnd;	// �G���h�n�_����̋���
		SearchState state;
		SearchPos();
		SearchPos(const Vector2Int& mapPos, const int start, const int end, const Vector2Int& parent, const SearchState state);
	};

	std::vector<SearchPos> _serchPosVec;

	const SearchPos GetSearchPos(const Vector2Int& mapPos)const;
	void SetSearchPos(const Vector2Int& nowPos, const int fromtStart, const Vector2Int& endMapPos, const Vector2Int& parent, const SearchState state);


public:
	Astar(const MapMng& mapMng);
	~Astar();

	// �J�n�ʒu����I�[�ʒu�܂ł̃}�X�����X�g�Ɋi�[���ĕԂ��B������Ȃ������ꍇ�̓��X�g����ɂ��ĕԂ�
	std::list<Vector2Int> RouteSearch(const Vector2Int& startMapPos, const Vector2Int& endMapPos);


};

