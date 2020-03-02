#pragma once
#include "Actor.h"
#include <string>

class MapCtrl;

class MapCursor :
	public Actor
{
protected:
	Vector2Int _mapPos;
	MapCtrl& _mapCtrl;

	int _moveItv;	// “ü—Íó•t‚Ü‚Å‚ÌƒtƒŒ[ƒ€”
	int _moveItvCurrentMax;
	const int _moveItvMax;

	int _putItv;
	int _putItvCurrentMax;
	const int _putItvMax;

	void CursorMove(const Input& input);

	bool PutCheck(const Input& input, const std::string& key);

public:
	MapCursor(MapCtrl& mapCtrl);
	~MapCursor();

	virtual void Update(const Input& input)override = 0;
	virtual void Draw(const Camera& camera)override = 0;

	Vector2Int GetMapPos()const;
};

