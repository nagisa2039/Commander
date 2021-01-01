#include <Dxlib.h>
#include <sstream>
#include "Map.h"
#include "FileSystem.h"
#include "DataBase.h"
#include "Camera.h"
#include "DxLibUtility.h"
#include "SoundLoader.h"

namespace
{
	constexpr unsigned int CHIP_SIZE_W = 64;
	constexpr unsigned int CHIP_SIZE_H = 64;

	constexpr unsigned int MAP_CHIP_CNT_W = 20;
	constexpr unsigned int MAP_CHIP_CNT_H = 20;

	constexpr double VERSION = 1.0;
}

void Map::DrawToMapScreen()
{
	int currentScreen = GetDrawScreen();
	SetDrawScreen(_mapGraphHandle);
	ClsDrawScreen();

	auto MapChipMax = DataBase::Instance().GetMapChipDataTable().size();

	SetDrawMode(DX_DRAWMODE_NEAREST);
	for (int y = 0;const auto& mapDataVec : _mapDataVec2)
	{
		for (int x = 0; const auto& mapData : mapDataVec)
		{
			DrawMapChip(Vector2Int{x,y}, MAP_CHIP_NONE);
			if (_mapDataVec2[y][x].mapChip > MAP_CHIP_NONE && _mapDataVec2[y][x].mapChip < MapChipMax)
			{
				DrawMapChip(Vector2Int{ x,y }, _mapDataVec2[y][x].mapChip);
			}
			x++;
		}
		y++;
	}
	SetDrawMode(DX_DRAWMODE_BILINEAR);

	SetDrawScreen(currentScreen);
}

void Map::DrawMapChip(const Vector2Int& mapPos, const Map_Chip mapChip, const Vector2Int& offset)
{
	auto& drawData = DataBase::Instance().GetMapChipData(mapChip).drawData;

	auto graphH = ImageHandle((imageFolderPath + drawData.path).c_str());
	DrawRectExtendGraph(offset.x + _chipSize.w * mapPos.x, offset.y + _chipSize.h * mapPos.y,
		offset.x + _chipSize.w * mapPos.x + _chipSize.w, offset.y + _chipSize.h * mapPos.y + _chipSize.h,
		drawData.leftup.x, drawData.leftup.y, drawData.size.w, drawData.size.h,
		graphH, true);
}

const std::string& Map::GetName() const
{
	return _name;
}

const int Map::GetMapGraphH() const
{
	return _mapGraphHandle;
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
	std::stringstream ss;
	ss << "Resource/Map/" << _fileName;

	FILE* fp = nullptr;

	fopen_s(&fp, ss.str().c_str(), "rb");

	TCHAR initPath[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, initPath);

	if (fp == NULL)
	{
		return false;
	}

	double version;
	fread_s(&version, sizeof(version), sizeof(version), 1, fp);
	if (version != VERSION)
	{
		fclose(fp);
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

Map::Map(const int mapId, std::string& name, std::string& fileName, std::string& bgmName)
	:_mapId(mapId), _mapSize{ MAP_CHIP_CNT_W, MAP_CHIP_CNT_H }, _chipSize{ CHIP_SIZE_W, CHIP_SIZE_H },
	imageFolderPath("Resource/Image/MapChip/"), _name(name), _fileName(fileName),_bgmName(bgmName)
{
	std::stringstream ss;
	ss << "map" << _mapId;
	auto& fileSystem = FileSystem::Instance();
	_mapGraphHandle = fileSystem.MakeScreen(ss.str().c_str(), _mapSize * _chipSize, true);
	LoadMapData();

	_bgmH = fileSystem.GetSoundHandle(bgmName.c_str());
}

Map::~Map()
{
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

void Map::StartBGM(const int volume)
{
	SoundL.PlayBGM(_bgmH, volume);
}

void Map::StopBGM()
{
	SoundL.StopSound(_bgmH);
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
	if (DataBase::Instance().GetMapChipData(_mapDataVec2[charactorChipInf.mapPos.y][charactorChipInf.mapPos.x].mapChip).moveCost < 0)
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