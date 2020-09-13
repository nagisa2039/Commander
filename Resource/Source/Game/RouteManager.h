#pragma once
#include <vector>
#include <list>
#include "Dir.h"
#include "Geometry.h"
#include "Astar.h"

class Charactor;
class MapCtrl;
class Camera;

class RouteManager
{
private:
	struct MoveInf
	{
		Dir dir;
		bool attack;
		Vector2Int mapPos;

		MoveInf() : dir(Dir::left), attack(false), mapPos(Vector2Int()) {};
		MoveInf(const Dir d, const bool at, const Vector2Int mp) : dir(d), attack(at), mapPos(mp) {};
	};

	std::vector<std::vector<std::list<Astar::ResultPos>>> _resultPosListVec2;
	std::list<MoveInf> _moveDirList;

	// �s���͈͓��̃L�����N�^�[���i�[
	std::list<Charactor*> _inRangeCharactorList;

	Charactor& _charactor;
	MapCtrl& _mapCtrl;
	Camera& _camera;

	// �w�肵�����W�ɉ��̃}�X�����邩���ׂ�	x=�ړ��}�X y=�U���}�X
	Vector2Int CheckMapPos(const Vector2Int& mapPos)const;

public:
	RouteManager(Charactor& charactor, MapCtrl& mapCtrl, Camera& camera);
	~RouteManager();

	void DrawMovableMass(const uint8_t alpha)const;
	bool GetAttackStartPos(Vector2Int& attackPos, const Vector2Int& targetMapPos)const;
	std::vector<std::vector<std::list<Astar::ResultPos>>>& GetResutlPosListVec2();
	std::list<MoveInf>& GetMoveDirList();

	// �w�肵�����W�܂ł̌o�H��`�悷��
	void DrawRoute(const Vector2Int& targetPos);
	// ���݂̏ꏊ����U���ł��鑊��̍��W��Ԃ�
	std::list<Vector2Int> GetAttackPosList()const;

	/// <summary>
	/// �w�肵���}�X�܂ł�ResultPosList���擾����
	/// </summary>
	std::list<Astar::ResultPos> CreateResultPosList(const Vector2Int& mapPos)const;

	/// <summary>
	/// resultPosList����MoveDirList���쐬����
	/// </summary>
	/// <param name="resultPosList">��ɂ���ResultPosList</param>
	void CreateMoveDirList(const std::list<Astar::ResultPos>& resultPosList);

	/// <summary>
	/// �͈͓��̃^�[�Q�b�g�L�����N�^�[���W�߂�
	/// </summary>
	void SearchRangeTarget();

	/// <summary>
	/// �s���͈͓��Ƀ^�[�Q�b�g�̃L�����N�^�[�����邩��Ԃ�
	/// </summary>
	bool CheckInRageTarget();

	/// <summary>
	/// �ړ��\�ȃ}�X��T��
	/// </summary>
	/// <param name="charactor"></param>
	void RouteSearch(Charactor& charactor);

	/// <summary>
	/// �w�肵���ړ��͂ōs����ꏊ��T��
	/// </summary>
	/// <param name="startPos"></param>
	/// <param name="move"></param>
	/// <param name="resultPosList"></param>
	/// <param name="team"></param>
	/// <param name="excludeList"></param>
	/// <returns></returns>
	bool MoveRouteSearch(const Vector2Int& startPos, const unsigned int move, 
		std::list<Astar::ResultPos>& resultPosList, const Team team, const std::list<Astar::ResultPos>& excludeList);

	/// <summary>
	/// �w��L�������王��͈͂ɂ���L�����N�^�[��Ԃ�
	/// </summary>
	/// <param name="charactor"></param>
	/// <returns></returns>
	Vector2Int SearchMovePos();
};

