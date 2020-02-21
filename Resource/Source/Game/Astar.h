#pragma once
#include <list>
#include <vector>
#include <array>
#include "../Utility/Geometry.h"

class Astar
{
public:
	struct ResultPos
	{
		bool attack;
		Vector2Int mapPos;

		ResultPos() :attack(false), mapPos(Vector2Int()) {};
		ResultPos(const bool atc, const Vector2Int mapP):attack(atc), mapPos(mapP) {};
	};

private:

	enum class SearchState
	{
		non,	// 未探索
		Serch,	// 探索中
		move	// 探索済み
	};

	enum Dir
	{
		left,
		right,
		up,
		down,
		max
	};

	struct SearchPos
	{
		Vector2Int mapPos;	//マップ上の座標
		Vector2Int parentPos;	// 親のマップ上の座標
		SearchState state;	// Search状況
		int moveCnt;	// 消費する移動量
		SearchPos();
		SearchPos(const Vector2Int& mapPos, const Vector2Int& parent, const SearchState state, const int moveCnt);
	};

	

	std::vector<std::vector<SearchPos>> _serchPosVec2;
	std::array<Vector2Int, Dir::max> _dirTable;

	void ResetSerchPosVec2D(const std::vector<std::vector<int>>& mapData);

public:
	Astar();
	~Astar();

	// 開始位置から終端位置までのマスをリストに格納して返す。見つからなかった場合はリストを空にして返す
	std::list<Astar::ResultPos> RouteSearch(const Vector2Int& startMapPos, const int move, const std::vector<std::vector<int>>& mapData);


};

