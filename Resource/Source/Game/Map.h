#pragma once
#include <vector>
#include <string>
#include "MapChip.h"

class Camera;

class Map
{
public:

private:
	const int _mapId;
	const std::string _name;
	const std::string _fileName;
	const std::string _bgmName;
	const Size _chipSize;
	const Size _mapSize;

	std::vector<std::vector<MapData>> _mapDataVec2;			// マップデータ

	int _mapGraphHandle;
	int _bgmH;

	const std::string imageFolderPath;

	bool LoadMapData();
	void DrawToMapScreen();

	// 指定したマスがマップの変更可能な範囲化を調べる
	bool CheckMapPosPutRange(const Vector2Int& mapPos);


public:
	Map(const int mapId, std::string& name, std::string& fileName, std::string& bgmName);
	~Map();

	void Draw(const Camera& camera);

	bool DrawMapChip(const Vector2Int& mapPos, const Map_Chip mapChip, const Vector2Int& offset = Vector2Int(0, 0));

	/// <summary>
	/// マップ名の取得
	/// </summary>
	const std::string& GetName()const;
	/// <summary>
	/// mapGraphHの取得
	/// </summary>
	const int GetMapGraphH()const;
	const int GetMapID()const;
	const Size& GetChipSize()const;
	const Size& GetMapSize()const;

	const std::vector<std::vector<MapData>>& GetMapData()const;
	const MapData& GetMapData(const Vector2Int& mapPos)const;
	CharactorChipInf GetCharactorChipInf(const Vector2Int& mapPos)const;
	bool SetCharactorChip(const CharactorChipInf& charactorChipInf);

	// 指定したマスがマップの範囲内かを返す
	bool CheckMapDataRange(const Vector2Int& mapPos)const;

	void StartBGM(const int volume = 255);
	void StopBGM();
};

