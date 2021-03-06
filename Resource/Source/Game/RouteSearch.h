#pragma once
#include <array>
#include "RouteSearchData.h"

/// <summary>
/// ルート検索
/// </summary>
class RouteSearch
{
private:
	/// <summary>
	/// 検索状況
	/// </summary>
	enum class SearchState
	{
		// 未探索
		non,
		// 探索
		search,
		// 攻撃
		attack
	};

	/// <summary>
	/// 検索中のマス
	/// </summary>
	struct SearchPos
	{
		//マップ上の座標
		Vector2Int mapPos;
		// 親のマップ上の座標
		Vector2Int parentPos;
		// Search状況
		SearchState state;
		// 消費する移動量
		int moveCost;
	};

	// マップデータを基に移動可能マス情報を管理
	std::vector<std::vector<SearchPos>> _searchPosVec2Move;
	// マップデータを基に攻撃可能マス情報を管理
	std::vector<std::vector<SearchPos>> _searchPosVec2Attack;
	// 方向ごとの移動量を管理するテーブル
	std::array<Vector2Int, static_cast<size_t>(Dir::max)> _dirTable;

	/// <summary>
	/// マップデータを基に検索情報をリセット
	/// </summary>
	/// <param name="mapData"></param>
	void ResetSerchPosVec2D(const SearchData::SearchMapData& mapData);

	/// <summary>
	/// マップ範囲外ではないか確認
	/// </summary>
	/// <param name="checkPos">確認座標</param>
	bool CheckSearchPosVec2Range(const Vector2Int& checkPos)const;

	/// <summary>
	/// 全ての移動可能範囲を検索
	/// </summary>
	/// <param name="startMapPos">開始座標</param>
	/// <param name="move">移動力</param>
	/// <param name="mapData">マップデータ</param>
	/// <param name="RouteSearchData">結果格納変数</param>
	/// <param name="team">チーム</param>
	/// <param name="heal">回復役か</param>
	/// <param name="searchEnemy">敵を探すか</param>
	void AllMoveRouteSerch(const Vector2Int& startMapPos, const int move, 
		const SearchData::SearchMapData& mapData, SearchData::RouteSearchData& routeSearchData, const Team team,
		const bool heal, const bool searchEnemy = false);

	/// <summary>
	/// _serchPosVec2MoveとcheckPosを基に移動済みのマスか確認する
	/// </summary>
	/// <param name="checkPos">確認座標</param>
	/// <param name="startMapPos">開始座標</param>
	/// <param name="nowPos">現在の座標</param>
	bool CheckMoved(const Vector2Int& checkPos, const Vector2Int& startMapPos, const Vector2Int& nowPos);

public:
	RouteSearch();
	~RouteSearch();

	/// <summary>
	/// 開始位置から終端位置までのマスをリストに格納して返す。見つからなかった場合はリストを空にして返す
	/// </summary>
	/// <param name="startMapPos">開始位置</param>
	/// <param name="move">移動力</param>
	/// <param name="attackRange">攻撃範囲</param>
	/// <param name="mapData">マップデータ</param>
	/// <param name="RouteSearchData">検索結果格納用配列</param>
	/// <param name="team">チーム</param>
	/// <param name="heal">回復役か</param>
	/// <param name="searchEnemy">移動範囲外の敵を探すか</param>
	void Search(const Vector2Int& startMapPos, const int move, const Range& attackRange, 
		const SearchData::SearchMapData& mapData, SearchData::RouteSearchData& routeSearchData,
		const Team team, const bool heal, const bool searchEnemy = false);

	/// <summary>
	/// 一つのルートを見つける
	/// </summary>
	/// <param name="startMapPos">開始位置</param>
	/// <param name="move">移動力</param>
	/// <param name="mapData">マップデータ</param>
	/// <param name="resutlPosListVec2">検索結果格納用配列</param>
	/// <param name="team">チーム</param>
	/// <param name="excludeList">除外するマスリスト</param>
	/// <returns>存在するか</returns>
	bool MoveRouteSerch(const Vector2Int& startMapPos, const int move, const SearchData::SearchMapData& mapData,
		SearchData::ResultList& resutlPosList, const Team team, const SearchData::ResultList& excludeList);
};

