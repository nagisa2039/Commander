#pragma once
#include "Actor.h"
#include "MapChip.h"

class MapCtrl;

class EditCursor :
	public Actor
{
private:
	MapCtrl& _mapCrtl;
	Map_Chip _mapChip;

	Vector2Int _mapPos;
	int _animCnt;


public:
	EditCursor(MapCtrl& mapCtrl);
	~EditCursor();

	void Update(const Input& input)override;
	void Draw(const Camera& camera)override;
};

