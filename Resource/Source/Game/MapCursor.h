#pragma once
#include "Actor.h"
#include <string>

class MapCtrl;

class MapCursor :
	public Actor
{
private:
	void KeybordMove(const Input& input);
	void MouseMove(const Input& input);

	void(MapCursor::* _cursorMover)(const Input&);

protected:
	Vector2Int _mapPos;
	MapCtrl& _mapCtrl;

	int _moveItv;	// “ü—Íó•t‚Ü‚Å‚ÌƒtƒŒ[ƒ€”
	int _moveItvCurrentMax;
	int _moveSEH;
	const int _moveItvMax;

	int _putItv;
	int _putItvCurrentMax;
	const int _putItvMax;

	void CursorMove(const Input& input);

	void ChangeKeybordInput();
	void ChangeMouseInput();

	bool PutCheck(const Input& input, const std::string& key);

	virtual void CursorMoveMoment();

public:
	MapCursor(MapCtrl& mapCtrl, Camera& camera);
	~MapCursor();

	virtual void Update(const Input& input)override = 0;
	virtual void Draw()override = 0;

	const Vector2Int& GetMapPos()const;
	void SetMapPos(const Vector2Int& mapPos);

	Vector2 GetCenterPos()const override;
};

