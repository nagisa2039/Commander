#include <algorithm>
#include "../System/Application.h"
#include "MapCursor.h"
#include "Camera.h"
#include "MapCtrl.h"
#include "../Utility/Input.h"
#include "SoundLoader.h"
#include "FileSystem.h"

using namespace std;

void MapCursor::KeybordMove(const Input& input)
{
	_moveItv--;
	auto oldMapPos = _mapPos;

	auto Move = [&](const std::string& key, const Vector2Int& move)
	{
		if (input.GetButton(key))
		{
			if (_moveItv > 0)
			{
				return true;
			}

			auto mapSize = _mapCtrl.GetMapSize();
			if ((oldMapPos + move).x >= 0 && (oldMapPos + move).x < mapSize.w
				&& (oldMapPos + move).y >= 0 && (oldMapPos + move).y < mapSize.h)
			{
				oldMapPos += move;
			}
			return true;
		}
		return false;
	};

	bool move = false;
	move |= Move("up", Vector2Int{ 0, -1 });
	move |= Move("down", Vector2Int{ 0, 1 });
	move |= Move("left", Vector2Int{ -1, 0 });
	move |= Move("right", Vector2Int{ 1, 0 });

	if (move)
	{
		if (_moveItv <= 0)
		{
			_moveItv = max(_moveItvCurrentMax / 2, 5);
			_moveItvCurrentMax = _moveItvCurrentMax / 2;
		}
	}
	else
	{
		_moveItv = 0;
		_moveItvCurrentMax = _moveItvMax;
	}

	if (_mapPos != oldMapPos)
	{
		_mapPos = oldMapPos;
		CursorMoveMoment();
	}
	_pos = (_mapPos * _mapCtrl.GetChipSize().ToVector2Int()).ToVector2();
}

void MapCursor::MouseMove(const Input& input)
{
	auto mousePos = input.GetMousePos();
	auto wsize = Application::Instance().GetWindowSize();
	
	_pos = (input.GetMousePos() - _camera.GetCameraOffset()).ToVector2();
	auto currentMapPos = _pos.ToVector2Int() / _mapCtrl.GetChipSize().ToVector2Int();

	// ”ÍˆÍŠO§Œä
	auto mapSize = _mapCtrl.GetMapSize();
	if (currentMapPos.x < 0 || currentMapPos.y < 0 
	 || currentMapPos.x >= mapSize.w || currentMapPos.y >= mapSize.h)
	{
		return;
	}

	if (currentMapPos != _mapPos)
	{
		_mapPos = currentMapPos;
		CursorMoveMoment();
	}
}

void MapCursor::CursorMove(const Input& input)
{
	if (input.GetAnyMouseInput())
	{
		ChangeMouseInput();
	}
	else if (input.GetAnyKeybordInput() || input.GetAnyPadInput())
	{
		ChangeKeybordInput();
	}
	(this->*_cursorMover)(input);
}

void MapCursor::ChangeKeybordInput()
{
	_cursorMover = &MapCursor::KeybordMove;
	_camera.SetLooseFollow(false);
}

void MapCursor::ChangeMouseInput()
{
	_cursorMover = &MapCursor::MouseMove;
	_camera.SetLooseFollow(true);
}

bool MapCursor::PutCheck(const Input& input, const std::string& key)
{
	_putItv--;
	if (input.GetButton(key))
	{
		if (_putItv <= 0)
		{
			_putItv = max(_putItvCurrentMax / 2, 5);
			_putItvCurrentMax = _putItvCurrentMax / 2;
			return true;
		}
	}
	else
	{
		_putItv = 0;
		_putItvCurrentMax = _putItvMax;
	}
	return false;
}

MapCursor::MapCursor(MapCtrl& mapCtrl, Camera& camera):_mapCtrl(mapCtrl), _moveItvMax(30), _putItvMax(30), Actor(camera)
{
	_pos = Vector2{};
	_mapPos = Vector2Int{};

	_moveItv = 0;
	_moveItvCurrentMax = _moveItvMax;

	_putItv = 0;
	_putItvCurrentMax = _putItvMax;

	_moveSEH = SoundHandle("Resource/Sound/SE/cursor.mp3");

	ChangeKeybordInput();
}

MapCursor::~MapCursor()
{
}

const Vector2Int& MapCursor::GetMapPos() const
{
	return _mapPos;
}

void MapCursor::SetMapPos(const Vector2Int& mapPos)
{
	_mapPos = mapPos;
}

Vector2 MapCursor::GetCenterPos() const
{
	return _cursorMover == &MapCursor::MouseMove ? _pos : _pos + _mapCtrl.GetChipSize().ToVector2() * 0.5f;
}

void MapCursor::CursorMoveMoment()
{
	SoundL.PlaySE(_moveSEH);
}
