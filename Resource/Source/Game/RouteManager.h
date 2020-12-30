#pragma once
#include <vector>
#include <list>
#include "Dir.h"
#include "Geometry.h"
#include "Astar.h"

class Charactor;
class MapCtrl;
class Camera;

/// <summary>
/// 経路管理クラス
/// </summary>
class RouteManager
{
private:
	/// <summary>
	/// 移動情報
	/// </summary>
	struct MoveInf
	{
		// 方向
		Dir dir;
		// 攻撃マスか
		bool attack;
		// マス数
		Vector2Int mapPos;
	};

	// 経路配列
	std::vector<std::vector<std::list<Astar::ResultPos>>> _resultPosListVec2;
	// 移動方向配列
	std::list<MoveInf> _moveDirList;

	// 行動範囲内のキャラクターを格納
	std::list<Charactor*> _inRangeCharactorList;

	// 所有者のキャラクター
	Charactor& _charactor;
	// マップ管理クラス
	MapCtrl& _mapCtrl;
	// カメラ
	Camera& _camera;

	/// <summary>
	/// 指定した座標に何のマスがあるか調べる	
	/// </summary>
	/// <param name="mapPos">マス数</param>
	/// <returns>x=移動マス y=攻撃マス</returns>
	Vector2Int CheckMapPos(const Vector2Int& mapPos)const;

public:
	/// <param name="charactor">所有者のキャラクター</param>
	/// <param name="mapCtrl">マップ管理クラス</param>
	/// <param name="camera">カメラ</param>
	RouteManager(Charactor& charactor, MapCtrl& mapCtrl, Camera& camera);
	~RouteManager();

	/// <summary>
	/// 移動範囲の描画
	/// </summary>
	/// <param name="alpha">不透明度</param>
	void DrawMovableMass(const uint8_t alpha)const;

	/// <summary>
	/// 指定した座標に攻撃可能なキャラクターがいるか確認
	/// いた場合は攻撃開始座標を取得
	/// </summary>
	/// <param name="attackPos">攻撃開始座標を取得するための変数</param>
	/// <param name="targetMapPos">確認する座標</param>
	bool GetAttackStartPos(Vector2Int& attackPos, const Vector2Int& targetMapPos)const;

	/// <summary>
	/// 経路探索の結果を格納した配列取得
	/// </summary>
	std::vector<std::vector<std::list<Astar::ResultPos>>>& GetResutlPosListVec2();

	/// <summary>
	/// 移動方向配列の取得
	/// </summary>
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

	/// <summary>
	/// 範囲内のターゲットキャラクターを集める
	/// </summary>
	void SearchRangeTarget();

	/// <summary>
	/// 行動範囲内にターゲットのキャラクターがいるかを返す
	/// </summary>
	bool CheckInRageTarget();

	/// <summary>
	/// 移動可能なマスを探す
	/// </summary>
	void RouteSearch();

	/// <summary>
	/// 指定した移動力で行ける場所を探す
	/// </summary>
	/// <param name="startPos">検索開始マス</param>
	/// <param name="move">移動力</param>
	/// <param name="resultPosList">結果格納変数</param>
	/// <param name="team">チーム</param>
	/// <param name="excludeList">除外するマスリスト</param>
	/// <returns>存在するか</returns>
	bool MoveRouteSearch(const Vector2Int& startPos, const unsigned int move, 
		std::list<Astar::ResultPos>& resultPosList, const Team team, const std::list<Astar::ResultPos>& excludeList);

	/// <summary>
	/// 
	/// </summary>
	Vector2Int SearchMovePos();
};

