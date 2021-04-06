#pragma once
#include <array>
#include "RouteSearchData.h"

/// <summary>
/// ���[�g����
/// </summary>
class RouteSearch
{
private:
	/// <summary>
	/// ������
	/// </summary>
	enum class SearchState
	{
		// ���T��
		non,
		// �T��
		search,
		// �U��
		attack
	};

	/// <summary>
	/// �������̃}�X
	/// </summary>
	struct SearchPos
	{
		//�}�b�v��̍��W
		Vector2Int mapPos;
		// �e�̃}�b�v��̍��W
		Vector2Int parentPos;
		// Search��
		SearchState state;
		// �����ړ���
		int moveCost;
	};

	// �}�b�v�f�[�^����Ɉړ��\�}�X�����Ǘ�
	std::vector<std::vector<SearchPos>> _searchPosVec2Move;
	// �}�b�v�f�[�^����ɍU���\�}�X�����Ǘ�
	std::vector<std::vector<SearchPos>> _searchPosVec2Attack;
	// �������Ƃ̈ړ��ʂ��Ǘ�����e�[�u��
	std::array<Vector2Int, static_cast<size_t>(Dir::max)> _dirTable;

	/// <summary>
	/// �}�b�v�f�[�^����Ɍ����������Z�b�g
	/// </summary>
	/// <param name="mapData"></param>
	void ResetSerchPosVec2D(const SearchData::SearchMapData& mapData);

	/// <summary>
	/// �}�b�v�͈͊O�ł͂Ȃ����m�F
	/// </summary>
	/// <param name="checkPos">�m�F���W</param>
	bool CheckSearchPosVec2Range(const Vector2Int& checkPos)const;

	/// <summary>
	/// �S�Ă̈ړ��\�͈͂�����
	/// </summary>
	/// <param name="startMapPos">�J�n���W</param>
	/// <param name="move">�ړ���</param>
	/// <param name="mapData">�}�b�v�f�[�^</param>
	/// <param name="RouteSearchData">���ʊi�[�ϐ�</param>
	/// <param name="team">�`�[��</param>
	/// <param name="heal">�񕜖���</param>
	/// <param name="searchEnemy">�G��T����</param>
	void AllMoveRouteSerch(const Vector2Int& startMapPos, const int move, 
		const SearchData::SearchMapData& mapData, SearchData::RouteSearchData& routeSearchData, const Team team,
		const bool heal, const bool searchEnemy = false);

	/// <summary>
	/// _serchPosVec2Move��checkPos����Ɉړ��ς݂̃}�X���m�F����
	/// </summary>
	/// <param name="checkPos">�m�F���W</param>
	/// <param name="startMapPos">�J�n���W</param>
	/// <param name="nowPos">���݂̍��W</param>
	bool CheckMoved(const Vector2Int& checkPos, const Vector2Int& startMapPos, const Vector2Int& nowPos);

public:
	RouteSearch();
	~RouteSearch();

	/// <summary>
	/// �J�n�ʒu����I�[�ʒu�܂ł̃}�X�����X�g�Ɋi�[���ĕԂ��B������Ȃ������ꍇ�̓��X�g����ɂ��ĕԂ�
	/// </summary>
	/// <param name="startMapPos">�J�n�ʒu</param>
	/// <param name="move">�ړ���</param>
	/// <param name="attackRange">�U���͈�</param>
	/// <param name="mapData">�}�b�v�f�[�^</param>
	/// <param name="RouteSearchData">�������ʊi�[�p�z��</param>
	/// <param name="team">�`�[��</param>
	/// <param name="heal">�񕜖���</param>
	/// <param name="searchEnemy">�ړ��͈͊O�̓G��T����</param>
	void Search(const Vector2Int& startMapPos, const int move, const Range& attackRange, 
		const SearchData::SearchMapData& mapData, SearchData::RouteSearchData& routeSearchData,
		const Team team, const bool heal, const bool searchEnemy = false);

	/// <summary>
	/// ��̃��[�g��������
	/// </summary>
	/// <param name="startMapPos">�J�n�ʒu</param>
	/// <param name="move">�ړ���</param>
	/// <param name="mapData">�}�b�v�f�[�^</param>
	/// <param name="resutlPosListVec2">�������ʊi�[�p�z��</param>
	/// <param name="team">�`�[��</param>
	/// <param name="excludeList">���O����}�X���X�g</param>
	/// <returns>���݂��邩</returns>
	bool MoveRouteSerch(const Vector2Int& startMapPos, const int move, const SearchData::SearchMapData& mapData,
		SearchData::ResultList& resutlPosList, const Team team, const SearchData::ResultList& excludeList);
};

