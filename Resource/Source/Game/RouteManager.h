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
};

