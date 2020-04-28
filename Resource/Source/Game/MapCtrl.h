#pragma once
#include <vector>
#include <string>
#include <array>
#include "../Utility/Geometry.h"
#include "MapChip.h"
#include <list>
#include <memory>
#include "Astar.h"

class Camera;
class Charactor;

class MapCtrl
{
public:
	struct DrawData
	{
		Vector2Int leftup;
		Size size;
		std::string path;

		DrawData() {};

		DrawData(Vector2Int leftup, Size size, std::string path) :
			leftup(leftup), size(size), path(path) {};

	};

	struct MapChipData
	{
		DrawData drawData;
		int moveCost;
		int defense;
		int avoidance;
		const char* name;

		MapChipData() : drawData(DrawData()), name(""), moveCost(1), defense(0), avoidance(0){};
		MapChipData(const DrawData& dd, const char* na) :
			drawData(dd), name(na), moveCost(1), defense(0), avoidance(0) {};
		MapChipData(const DrawData& dd, const char* na, const int mc) :
			drawData(dd), name(na), moveCost(mc), defense(0), avoidance(0) {};
		MapChipData(const DrawData& dd, const char* na, const int mc, const int fe, const int av) :
			drawData(dd), name(na), moveCost(mc), defense(fe), avoidance(av) {};
	};

private:
	std::vector<std::vector<Map_Chip>> _mapData;			// マップデータ
	std::array<MapChipData, Map_Chip_Max> _mapChipData;	// マップチップのデータ
	std::shared_ptr<Astar> _astar;

	std::vector<std::shared_ptr<Charactor>>& _charactors;

	int _mapFloorH;
	int _mapChipH;
	const std::string imageFolderPath;

	void DrawToMapFloorScreen();
	void DrawToMapChipScreen();

public:
	MapCtrl(std::vector<std::shared_ptr<Charactor>>& charactors);
	~MapCtrl();

	void Draw(const Camera& camera, const bool edit = false);

	Size GetChipSize()const;
	Size GetMapCnt()const;

	// そのマスにいるキャラクターを返す
	Charactor* GetMapPosChar(const Vector2Int mapPos);

	bool SetMapChip(const Vector2Int& mapPos, const Map_Chip mapChip);
	bool DrawMapChip(const Vector2Int& mapPos, const Map_Chip mapChip, const Vector2Int& offset = Vector2Int(0,0));

	bool SaveMap(const std::string fileName);
	bool LoadMap(const std::string fileName);

	// 移動可能なマスを探す
	void RouteSearch(Charactor& charactor);

	// 指定キャラから視野範囲にいるキャラクターを返す
	Vector2Int SearchMovePos(Charactor& charactor);

	void CreateMapVec(std::vector<std::vector<Astar::MapData>>& mapVec2, const Team team);

	// 指定した座標のMapChipDataを取得する
	MapChipData GetMapChipData(const Vector2Int& mapPos)const;
};

