#pragma once
#include <vector>
#include <string>
#include <array>
#include "../Utility/Geometry.h"
#include "MapChip.h"

class Camera;

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

private:
	std::vector<std::vector<Map_Chip>> _mapData;			// マップデータ
	std::array<DrawData, Map_Chip_Max> _mapChipDrawData;	// DrawRectGraphの時に使うデータ

	int _mapFloorH;
	int _mapChipH;
	const std::string imageFolderPath;

	void DrawToMapFloorScreen();
	void DrawToMapChipScreen();

public:
	MapCtrl();
	~MapCtrl();

	void Draw(const Camera& camera, const bool edit = false);

	Size GetChipSize()const;
	Size GetMapSize()const;

	bool SetMapChip(const Vector2Int& mapPos, const Map_Chip mapChip);
	bool DrawMapChip(const Vector2Int& mapPos, const Map_Chip mapChip, const Vector2Int& offset = Vector2Int(0,0));

	bool SaveMap(const std::string fileName);
	bool LoadMap(const std::string fileName);
};

