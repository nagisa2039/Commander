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
		non,	// 未探索
		Serch,	// 探索中
		move	// 探索済み
	};

	struct SearchPos
	{
		Vector2Int mapPos;
		Vector2Int parentPos;	// 親のマス
		int fromStart;	// スタート地点からの距離
		int fromEnd;	// エンド地点からの距離
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

	// 開始位置から終端位置までのマスをリストに格納して返す。見つからなかった場合はリストを空にして返す
	std::list<Vector2Int> RouteSearch(const Vector2Int& startMapPos, const Vector2Int& endMapPos);


};

