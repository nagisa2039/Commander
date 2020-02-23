#pragma once
#include "Actor.h"

class MapCtrl;

class MapCursor :
	public Actor
{
protected:
	Vector2Int _mapPos;
	MapCtrl& _mapCtrl;

	void CursorMove(const Input& input);

public:
	MapCursor(MapCtrl& mapCtrl);
	~MapCursor();

	virtual void Update(const Input& input)override = 0;
	virtual void Draw(const Camera& camera)override = 0;
};

