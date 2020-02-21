#include "MapCtrl.h"
#include "../Utility/DxLibUtility.h"
#include "../System/Application.h"
#include "../System/FileSystem.h"
#include "../Game/Camera.h"

using namespace std;

constexpr auto CHIP_SIZE_W = 32;
constexpr auto CHIP_SIZE_H = 32;


void MapCtrl::DrawToMapChipScreen()
{
	SetDrawScreen(_mapChipH);
	ClsDrawScreen();

	for (int y = 0; y < _mapData.size(); y++)
	{
		for (int x = 0; x < _mapData[y].size(); x++)
		{
			if (_mapData[y][x] > None && _mapData[y][x] < Map_Chip_Max)
			{
				DrawMapChip(Vector2Int(x, y), _mapData[y][x]);
			}
		}
	}

	SetDrawScreen(DX_SCREEN_BACK);
}

void MapCtrl::DrawToMapFloorScreen()
{
	SetDrawScreen(_mapFloorH);
	ClsDrawScreen();

	for (int y = 0; y < _mapData.size(); y++)
	{
		for (int x = 0; x < _mapData[y].size(); x++)
		{
			DrawMapChip(Vector2Int(x, y), Floor_Meadow);
		}
	}

	SetDrawScreen(DX_SCREEN_BACK);
}

MapCtrl::MapCtrl() : imageFolderPath("Resource/Image/MapChip/")
{
	_mapChipH = MakeScreen(100 * CHIP_SIZE_W, 100 * CHIP_SIZE_H, true);
	_mapFloorH = MakeScreen(100 * CHIP_SIZE_W, 100 * CHIP_SIZE_H, true);

	_mapData.resize(20);
	for (auto& mapDataX : _mapData)
	{
		mapDataX.resize(25);
		for (auto& mapData : mapDataX)
		{
			mapData = None;
		}
	}

	_mapChipDrawData[Floor_Meadow]			 = DrawData(Vector2Int(0, 0),	Size(32, 32), "mapchip0.png");
	_mapChipDrawData[Forest]			 = DrawData(Vector2Int(32, 32), Size(32, 32), "mapchip0.png");
	_mapChipDrawData[River_Pond]		 = DrawData(Vector2Int(0, 0),	Size(32, 32), "mapchip1.png");
	_mapChipDrawData[River_Vertical]	 = DrawData(Vector2Int(0, 32),	Size(32, 32), "mapchip1.png");
	_mapChipDrawData[River_Horizontal]	 = DrawData(Vector2Int(0, 64),	Size(32, 32), "mapchip1.png");
	_mapChipDrawData[River_Cross]		 = DrawData(Vector2Int(0, 96),	Size(32, 32), "mapchip1.png");
	_mapChipDrawData[River_All]			 = DrawData(Vector2Int(0, 128), Size(32, 32), "mapchip1.png");
	_mapChipDrawData[River_Corner0]		 = DrawData(Vector2Int(0, 160), Size(32, 32), "mapchip1.png");
	_mapChipDrawData[River_Corner1]		 = DrawData(Vector2Int(0, 192), Size(32, 32), "mapchip1.png");
	_mapChipDrawData[River_Corner2]		 = DrawData(Vector2Int(0, 224), Size(32, 32), "mapchip1.png");
	_mapChipDrawData[River_Corner3]		 = DrawData(Vector2Int(0, 256), Size(32, 32), "mapchip1.png");

	DrawToMapFloorScreen();
	DrawToMapChipScreen();
}

MapCtrl::~MapCtrl()
{
}

void MapCtrl::Draw(const Camera& camera, const bool edit)
{
	auto offset = camera.GetCameraOffset();

	DrawGraph(offset, _mapFloorH);
	DrawGraph(offset, _mapChipH);

	if (edit)
	{
		auto mapSize = GetMapSize();
		for (int h = 0; h <= mapSize.h; h++)
		{
			DrawLine(Vector2Int(0, h * CHIP_SIZE_H) + offset, 
				Vector2Int(mapSize.w * CHIP_SIZE_W, h * CHIP_SIZE_H) + offset, 0xffffff);
		}
		for (int w = 0; w <= mapSize.w; w++)
		{
			DrawLine(Vector2Int(w * CHIP_SIZE_W, 0) + offset, 
				Vector2Int(w * CHIP_SIZE_W, mapSize.h * CHIP_SIZE_H) + offset, 0xffffff);
		}
	}
}

Size MapCtrl::GetChipSize()const
{
	return Size(CHIP_SIZE_W, CHIP_SIZE_H);
}

Size MapCtrl::GetMapSize() const
{
	return Size(_mapData[0].size(), _mapData.size());
}

bool MapCtrl::SetMapChip(const Vector2Int& mapPos, const Map_Chip mapChip)
{
	if (mapPos.x >= 0 && mapPos.x < _mapData[0].size()
		&& mapPos.y >= 0 && mapPos.y < _mapData.size())
	{
		_mapData[mapPos.y][mapPos.x] = mapChip;
		DrawToMapChipScreen();
		return true;
	}
	return false;
}

bool MapCtrl::DrawMapChip(const Vector2Int& mapPos, const Map_Chip mapChip, const Vector2Int& offset)
{
	auto drawData = _mapChipDrawData[mapChip];

	auto graphH = Application::Instance().GetFileSystem()->GetImageHandle((imageFolderPath + drawData.path).c_str());
	DrawRectGraph(offset.x + CHIP_SIZE_W * mapPos.x, offset.y + CHIP_SIZE_H * mapPos.y,
		drawData.leftup.x, drawData.leftup.y, drawData.size.w, drawData.size.h,
		graphH, true);

	return true;
}

bool MapCtrl::SaveMap(const std::string fileName)
{
	FILE* fp = nullptr;

	string folderName("Resource/Map/");
	fopen_s(&fp, (folderName + fileName).c_str(), "wb");

	if (fp == NULL)
	{
		return false;
	}

	auto mapSize = GetMapSize();
	fwrite(&mapSize, sizeof(mapSize), 1, fp);

	for (const auto& mapChipVec : _mapData)
	{
		fwrite(mapChipVec.data(), sizeof(Map_Chip), mapChipVec.size(), fp);
	}

	fclose(fp);

	return true;
}

bool MapCtrl::LoadMap(const std::string fileName)
{
	FILE* fp = nullptr;

	string folderName("Resource/Map/");
	fopen_s(&fp, (folderName + fileName).c_str(), "rb");

	if (fp == NULL)
	{
		return false;
	}

	Size mapSize;
	fread_s(&mapSize, sizeof(mapSize), sizeof(mapSize), 1, fp);
	_mapData.resize(mapSize.h);
	for (auto& mapChipVec : _mapData)
	{
		mapChipVec.resize(mapSize.w);
		fread_s(mapChipVec.data(), sizeof(Map_Chip) * mapSize.w, sizeof(Map_Chip), mapSize.w, fp);
	}

	fclose(fp);

	return true;
}

