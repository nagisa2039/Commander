#include <Dxlib.h>
#include <sstream>
#include "Map.h"
#include "Application.h"
#include "FileSystem.h"
#include "DataBase.h"
#include "Camera.h"
#include "DxLibUtility.h"

namespace
{
	constexpr unsigned int CHIP_SIZE_W = 80;
	constexpr unsigned int CHIP_SIZE_H = 80;

	constexpr unsigned int MAP_CHIP_CNT_W = 20;
	constexpr unsigned int MAP_CHIP_CNT_H = 20;
}

void Map::DrawToMapScreen()
{
	int currentScreen = GetDrawScreen();
	SetDrawScreen(_mapGraphHandle);
	ClsDrawScreen();

	int y = 0;
	for (const auto& mapDataVec : _mapDataVec2)
	{
		int x = 0;
		for (const auto& mapData : mapDataVec)
		{
			DrawMapChip(Vector2Int(x, y), Map_Chip::none);
			if (_mapDataVec2[y][x].mapChip > Map_Chip::none && _mapDataVec2[y][x].mapChip < Map_Chip::max)
			{
				DrawMapChip(Vector2Int(x, y), _mapDataVec2[y][x].mapChip);
			}
			x++;
		}
		y++;
	}

	SetDrawScreen(currentScreen);
}

bool Map::DrawMapChip(const Vector2Int& mapPos, const Map_Chip mapChip, const Vector2Int& offset)
{
	auto drawData = Application::Instance().GetDataBase().GetMapChipData(mapChip).drawData;

	auto graphH = Application::Instance().GetFileSystem().GetImageHandle((imageFolderPath + drawData.path).c_str());
	DrawRectExtendGraph(offset.x + _chipSize.w * mapPos.x, offset.y + _chipSize.h * mapPos.y,
		offset.x + _chipSize.w * mapPos.x + _chipSize.w, offset.y + _chipSize.h * mapPos.y + _chipSize.h,
		drawData.leftup.x, drawData.leftup.y, drawData.size.w, drawData.size.h,
		graphH, true);

	return true;
}

const int Map::GetMapID() const
{
	return _mapId;
}

const Size& Map::GetChipSize() const
{
	return _chipSize;
}

const Size& Map::GetMapSize() const
{
	return _mapSize;
}

bool Map::LoadMapData()
{
	auto mapData = Application::Instance().GetDataBase().GetMapData(_mapId);
	std::stringstream ss;
	ss << "Resource/Map/" << mapData.fileName;

	FILE* fp = nullptr;

	fopen_s(&fp, ss.str().c_str(), "rb");

	TCHAR initPath[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, initPath);

	if (fp == NULL)
	{
		return false;
	}

	// マップチップの読み込み
	Size mapSize;
	fread_s(&mapSize, sizeof(mapSize), sizeof(mapSize), 1, fp);

	if (mapSize != _mapSize)
	{
		fclose(fp);
		return false;
	}

	_mapDataVec2.resize(mapSize.h);
	auto ms = sizeof(Map_Chip);
	auto cs = sizeof(CharactorChipInf);
	auto mapDataSize = sizeof(MapData);
	for (auto& mapChipVec : _mapDataVec2)
	{
		mapChipVec.resize(mapSize.w);
		fread_s(mapChipVec.data(), mapDataSize * mapSize.w, sizeof(MapData), mapSize.w, fp);
	}

	fclose(fp);

	DrawToMapScreen();

	return true;
}

Map::Map(const int mapId)
	:_mapId(mapId), _mapSize(MAP_CHIP_CNT_W, MAP_CHIP_CNT_H), _chipSize(CHIP_SIZE_W, CHIP_SIZE_H),
	imageFolderPath("Resource/Image/MapChip/")
{
	_mapGraphHandle = MakeScreen(_mapSize.w * _chipSize.w, _mapSize.h * _chipSize.h, true);
	assert(LoadMapData());
}

Map::~Map()
{
	DeleteGraph(_mapGraphHandle);
}

void Map::Draw(const Camera& camera)
{
	auto offset = camera.GetCameraOffset();
	DrawGraph(offset, _mapGraphHandle);
}

bool Map::CheckMapPosPutRange(const Vector2Int& mapPos)
{
	int frameNum = 0;
	return mapPos.y >= frameNum && mapPos.y < static_cast<int>(MAP_CHIP_CNT_H - frameNum)
		&& mapPos.x >= frameNum && mapPos.x < static_cast<int>(MAP_CHIP_CNT_W - frameNum);
}

const std::vector<std::vector<MapData>>& Map::GetMapData() const
{
	return _mapDataVec2;
}

const MapData& Map::GetMapData(const Vector2Int& mapPos) const
{
	return _mapDataVec2[mapPos.y][mapPos.x];
}

CharactorChipInf Map::GetCharactorChipInf(const Vector2Int& mapPos) const
{
	return _mapDataVec2[mapPos.y][mapPos.x].charactorChip;
}

bool Map::SetCharactorChip(const CharactorChipInf& charactorChipInf)
{
	// 移動不可のマスには設置できないようにする
	if (Application::Instance().GetDataBase().GetMapChipData(_mapDataVec2[charactorChipInf.mapPos.y][charactorChipInf.mapPos.x].mapChip).moveCost < 0)
	{
		return false;
	}

	_mapDataVec2[charactorChipInf.mapPos.y][charactorChipInf.mapPos.x].charactorChip = charactorChipInf;
	return true;
}

bool Map::CheckMapDataRange(const Vector2Int& mapPos)const
{
	return mapPos.x >= 0 && mapPos.x < _mapDataVec2[mapPos.y].size() && mapPos.y >= 0 && mapPos.y < _mapDataVec2.size();
}