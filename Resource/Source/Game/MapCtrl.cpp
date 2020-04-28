#include "MapCtrl.h"
#include "../Utility/DxLibUtility.h"
#include "../System/Application.h"
#include "../System/FileSystem.h"
#include "../Game/Camera.h"
#include "Charactor.h"
#include "DxLib.h"
#include <algorithm>

using namespace std;

constexpr auto CHIP_SIZE_W = 32*2;
constexpr auto CHIP_SIZE_H = 32*2;

constexpr auto MAP_CHIP_CNT_W = 30;
constexpr auto MAP_CHIP_CNT_H = 20;

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

MapCtrl::MapCtrl(std::vector<std::shared_ptr<Charactor>>& charactors) : _charactors(charactors), imageFolderPath("Resource/Image/MapChip/")
{
	_astar = make_shared<Astar>();
	_mapChipH = MakeScreen(100 * CHIP_SIZE_W, 100 * CHIP_SIZE_H, true);
	_mapFloorH = MakeScreen(100 * CHIP_SIZE_W, 100 * CHIP_SIZE_H, true);

	_mapData.resize(MAP_CHIP_CNT_H);
	for (auto& mapDataX : _mapData)
	{
		mapDataX.resize(MAP_CHIP_CNT_W);
		for (auto& mapData : mapDataX)
		{
			mapData = None;
		}
	}

	_mapChipData[None]				= MapChipData(DrawData(Vector2Int(0, 0), Size(32, 32), "mapchip0.png"), "ëêå¥", +1);
	_mapChipData[Floor_Meadow]		= MapChipData( DrawData(Vector2Int(0, 0),	Size(32, 32), "mapchip0.png"), "ëêå¥", +1);
	_mapChipData[Forest]			= MapChipData( DrawData(Vector2Int(32, 32), Size(32, 32), "mapchip0.png"), "êX", +2, 0, 30);
	_mapChipData[River_Pond]		= MapChipData( DrawData(Vector2Int(0, 0),	Size(32, 32), "mapchip1.png"), "êÏ",  -1, 0, 0);
	_mapChipData[River_Vertical]	= MapChipData( DrawData(Vector2Int(0, 32),	Size(32, 32), "mapchip1.png"), "êÏ", -1);
	_mapChipData[River_Horizontal]	= MapChipData( DrawData(Vector2Int(0, 64),	Size(32, 32), "mapchip1.png"), "êÏ", -1);
	_mapChipData[River_Cross]		= MapChipData( DrawData(Vector2Int(0, 96),	Size(32, 32), "mapchip1.png"), "êÏ", -1);
	_mapChipData[River_All]			= MapChipData( DrawData(Vector2Int(0, 128), Size(32, 32), "mapchip1.png"), "êÏ", -1);
	_mapChipData[River_Corner0]		= MapChipData( DrawData(Vector2Int(0, 160), Size(32, 32), "mapchip1.png"), "êÏ", -1);
	_mapChipData[River_Corner1]		= MapChipData( DrawData(Vector2Int(0, 192), Size(32, 32), "mapchip1.png"), "êÏ", -1);
	_mapChipData[River_Corner2]		= MapChipData( DrawData(Vector2Int(0, 224), Size(32, 32), "mapchip1.png"), "êÏ", -1);
	_mapChipData[River_Corner3]		= MapChipData( DrawData(Vector2Int(0, 256), Size(32, 32), "mapchip1.png"), "êÏ", -1);

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
		auto mapSize = GetMapCnt();
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

Size MapCtrl::GetMapCnt() const
{
	return Size(_mapData[0].size(), _mapData.size());
}

Charactor* MapCtrl::GetMapPosChar(const Vector2Int mapPos)
{
	for (auto& charactor : _charactors)
	{
		if (mapPos == charactor->GetMapPos())
		{
			return &(*charactor);
		}
	}
	return nullptr;
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
	auto drawData = _mapChipData[mapChip].drawData;

	auto graphH = Application::Instance().GetFileSystem()->GetImageHandle((imageFolderPath + drawData.path).c_str());
	DrawRectExtendGraph(offset.x + CHIP_SIZE_W * mapPos.x, offset.y + CHIP_SIZE_H * mapPos.y,
		offset.x + CHIP_SIZE_W * mapPos.x + CHIP_SIZE_W, offset.y + CHIP_SIZE_H * mapPos.y + CHIP_SIZE_H,
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

	auto mapSize = GetMapCnt();
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

	DrawToMapChipScreen();

	return true;
}

void MapCtrl::RouteSearch(Charactor& charactor)
{
	std::vector<std::vector<Astar::MapData>> mapVec2;
	CreateMapVec(mapVec2, charactor.GetTeam());

	return _astar->RouteSearch(charactor.GetMapPos(), charactor.GetStatus().move, charactor.GetAttackRange(), mapVec2, charactor.GetResutlPosList(), charactor.GetTeam());
}

Vector2Int MapCtrl::SearchMovePos(Charactor& charactor)
{
	std::vector<std::vector<Astar::MapData>> mapVec2;
	CreateMapVec(mapVec2, charactor.GetTeam());
	auto status = charactor.GetStatus();

	auto& resultPosList = charactor.GetResutlPosList();
	_astar->RouteSearch(charactor.GetMapPos(), status.move*2, charactor.GetAttackRange(), mapVec2, resultPosList, charactor.GetTeam());

	for (const auto& resultPos : resultPosList)
	{ 
		// çUåÇÉ}ÉXÇ…Ç»ÇÈÇ‹Ç≈continue
		if (!resultPos.attack)
		{
			continue;
		}

		// çUåÇÉ}ÉXÇ…ìGÉLÉÉÉâÇ™Ç¢ÇÈÇ©
		auto mapCharactor = GetMapPosChar(resultPos.mapPos);
		if (mapCharactor != nullptr && charactor.GetTeam() != mapCharactor->GetTeam())
		{
			return resultPos.mapPos;
		}
	}

	return Vector2Int(-1,-1);
}

void MapCtrl::CreateMapVec(std::vector<std::vector<Astar::MapData>>& mapVec2, const Team team)
{
	mapVec2.resize(_mapData.size());
	for (int y = 0; y < mapVec2.size(); y++)
	{
		mapVec2[y].resize(_mapData[y].size());
		for (int x = 0; x < mapVec2[y].size(); x++)
		{
			mapVec2[y][x] = Astar::MapData(_mapChipData[_mapData[y][x]].moveCost, Team::max);
		}
	}
	for (const auto& charactor : _charactors)
	{
		auto mapPos = charactor->GetMapPos();
		mapVec2[mapPos.y][mapPos.x].team = charactor->GetTeam();
	}
}

MapCtrl::MapChipData MapCtrl::GetMapChipData(const Vector2Int& mapPos) const
{
	if ( 0 > mapPos.x && MAP_CHIP_CNT_W <= mapPos.x
	  && 0 > mapPos.y && MAP_CHIP_CNT_H <= mapPos.y)
	{
		return MapChipData();
	}

	Map_Chip mapChip = _mapData[mapPos.y][mapPos.x];
	if (mapChip < 0 && mapChip >= Map_Chip_Max)
	{
		return MapChipData();
	}

	return _mapChipData[mapChip];
}
//
//void MapCtrl::MapChipData::operator=(const MapChipData& mcd)
//{
//	this->drawData = mcd.drawData;
//	this->moveCost = mcd.moveCost;
//}
