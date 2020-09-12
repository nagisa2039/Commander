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

	// 指定した座標に何のマスがあるか調べる	x=移動マス y=攻撃マス
	Vector2Int CheckMapPos(const Vector2Int& mapPos)const;

public:
	RouteManager(Charactor& charactor, MapCtrl& mapCtrl, Camera& camera);
	~RouteManager();

	void DrawMovableMass(const uint8_t alpha)const;
	bool GetAttackStartPos(Vector2Int& attackPos, const Vector2Int& targetMapPos)const;
	std::vector<std::vector<std::list<Astar::ResultPos>>>& GetResutlPosListVec2();
	std::list<MoveInf>& GetMoveDirList();

	// 指定した座標までの経路を描画する
	void DrawRoute(const Vector2Int& targetPos);
	// 現在の場所から攻撃できる相手の座標を返す
	std::list<Vector2Int> GetAttackPosList()const;

	/// <summary>
	/// 指定したマスまでのResultPosListを取得する
	/// </summary>
	std::list<Astar::ResultPos> CreateResultPosList(const Vector2Int& mapPos)const;

	/// <summary>
	/// resultPosListからMoveDirListを作成する
	/// </summary>
	/// <param name="resultPosList">基にするResultPosList</param>
	void CreateMoveDirList(const std::list<Astar::ResultPos>& resultPosList);
};

