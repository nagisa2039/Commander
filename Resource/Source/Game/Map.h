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
	const Size _chipSize;
	const Size _mapSize;

	std::vector<std::vector<MapData>> _mapDataVec2;			// �}�b�v�f�[�^

	int _mapGraphHandle;

	const std::string imageFolderPath;

	bool LoadMapData();
	void DrawToMapScreen();

	// �w�肵���}�X���}�b�v�̕ύX�\�Ȕ͈͉��𒲂ׂ�
	bool CheckMapPosPutRange(const Vector2Int& mapPos);

public:
	Map(const int mapId);
	~Map();

	void Draw(const Camera& camera);

	bool DrawMapChip(const Vector2Int& mapPos, const Map_Chip mapChip, const Vector2Int& offset = Vector2Int(0, 0));

	const int GetMapID()const;
	const Size& GetChipSize()const;
	const Size& GetMapSize()const;

	const std::vector<std::vector<MapData>>& GetMapData()const;
	const MapData& GetMapData(const Vector2Int& mapPos)const;
	CharactorChipInf GetCharactorChipInf(const Vector2Int& mapPos)const;
	bool SetCharactorChip(const CharactorChipInf& charactorChipInf);

	// �w�肵���}�X���}�b�v�͈͓̔�����Ԃ�
	bool CheckMapDataRange(const Vector2Int& mapPos)const;
};

