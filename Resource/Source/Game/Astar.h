#pragma once
#include <list>
#include <vector>
#include <array>
#include <map>
#include "../Utility/Geometry.h"
#include "../Utility/Dir.h"
#include "Team.h"

/// <summary>
/// ���[�g����
/// </summary>
class Astar
{
public:
	/// <summary>
	/// �}�X���Ƃ̌�������
	/// </summary>
	struct ResultPos
	{
		// �U���}�X��
		bool attack;
		// �}�b�v���W
		Vector2Int mapPos;
		// �X�^�[�g���炽�ǂ������̐e
		ResultPos* prev;
		// �X�^�[�g���炽�ǂ������̈ړ�����
		Dir dir;
		// �����ړ���
		int moveCnt;
	};

	// �}�b�v���
	struct MapData
	{
		// �ړ��R�X�g
		int moveCost;
		// �ǂ̃`�[���̃L�����N�^�[�����邩
		Team team;
		// �_���[�W���󂯂Ă��邩
		bool isHurt;
	};

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
	void ResetSerchPosVec2D(const std::vector<std::vector<MapData>>& mapData);

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
	/// <param name="resutlPosListVec2">���ʊi�[�ϐ�</param>
	/// <param name="team">�`�[��</param>
	/// <param name="heal">�񕜖���</param>
	/// <param name="searchEnemy">�G��T����</param>
	void AllMoveRouteSerch(const Vector2Int& startMapPos, const int move, 
		const std::vector<std::vector<MapData>>& mapData, std::vector<std::vector<std::list<Astar::ResultPos>>>& resutlPosListVec2, const Team team, const bool heal, const bool searchEnemy = false);

	/// <summary>
	/// _serchPosVec2Move��checkPos����Ɉړ��ς݂̃}�X���m�F����
	/// </summary>
	/// <param name="checkPos">�m�F���W</param>
	/// <param name="startMapPos">�J�n���W</param>
	/// <param name="nowPos">���݂̍��W</param>
	bool CheckMoved(const Vector2Int& checkPos, const Vector2Int& startMapPos, const Vector2Int& nowPos);

public:
	Astar();
	~Astar();

	/// <summary>
	/// �J�n�ʒu����I�[�ʒu�܂ł̃}�X�����X�g�Ɋi�[���ĕԂ��B������Ȃ������ꍇ�̓��X�g����ɂ��ĕԂ�
	/// </summary>
	/// <param name="startMapPos">�J�n�ʒu</param>
	/// <param name="move">�ړ���</param>
	/// <param name="attackRange">�U���͈�</param>
	/// <param name="mapData">�}�b�v�f�[�^</param>
	/// <param name="resutlPosListVec2">�������ʊi�[�p�z��</param>
	/// <param name="team">�`�[��</param>
	/// <param name="heal">�񕜖���</param>
	/// <param name="searchEnemy">�ړ��͈͊O�̓G��T����</param>
	void RouteSearch(const Vector2Int& startMapPos, const int move, const Range& attackRange, 
		const std::vector<std::vector<MapData>>& mapData, std::vector<std::vector<std::list<Astar::ResultPos>>>& resutlPosListVec2, const Team team, const bool heal, const bool searchEnemy = false);

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
	bool MoveRouteSerch(const Vector2Int& startMapPos, const int move, const std::vector<std::vector<MapData>>& mapData, std::list<Astar::ResultPos>& resutlPosList, const Team team, const std::list<Astar::ResultPos>& excludeList);
};

