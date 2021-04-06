#pragma once
#include <vector>
#include <list>
#include "../Utility/Geometry.h"
#include "../Utility/Dir.h"
#include "Team.h"

namespace SearchData
{
	/// <summary>
	/// マスごとの検索結果
	/// </summary>
	struct ResultPos
	{
		// 攻撃マスか
		bool attack;
		// マップ座標
		Vector2Int mapPos;
		// スタートからたどった時の親
		ResultPos* prev;
		// スタートからたどった時の移動方向
		Dir dir;
		// 消費する移動量
		int moveCnt;
	};

	// マップチップ情報
	struct MapChipData
	{
		// 移動コスト
		int moveCost;
		// どのチームのキャラクターがいるか
		Team team;
		// ダメージを受けているか
		bool isHurt;
	};

	// 検索用マップ情報
	using SearchMapData = std::vector<std::vector<MapChipData>>;
	// １マス分の検索結果
	using ResultList = std::list<ResultPos>;
	// マップデータ分の検索結果情報
	using RouteSearchData = std::vector<std::vector<ResultList>>;
}