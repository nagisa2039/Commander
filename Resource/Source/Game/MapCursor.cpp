#include "MapCursor.h"
#include "MapCtrl.h"
#include "../Utility/Input.h"
#include <algorithm>

using namespace std;

void MapCursor::CursorMove(const Input& input)
{
	_moveItv--;
	auto Move = [&](const std::string& key, const Vector2Int& move)
	{
		if (input.GetButton(0, key))
		{
			if (_moveItv > 0)
			{
				return true;
			}

			auto mapSize = _mapCtrl.GetMapCnt();
			if ((_mapPos + move).x >= 0 && (_mapPos + move).x < mapSize.w
				&& (_mapPos + move).y >= 0 && (_mapPos + move).y < mapSize.h)
			{
				_mapPos += move;
			}
			return true;
		}
		return false;
	};

	bool move = false;
	move |= Move("up", Vector2Int(0, -1));
	move |= Move("down", Vector2Int(0, 1));
	move |= Move("left", Vector2Int(-1, 0));
	move |= Move("right", Vector2Int(1, 0));

	if(move)
	{
		if (_moveItv <= 0)
		{
			_moveItv = std::max(_moveItvCurrentMax / 2, 5);
			_moveItvCurrentMax = _moveItvCurrentMax /2;
		}
	}
	else
	{
		_moveItv = 0;
		_moveItvCurrentMax = _moveItvMax;
	}
	_pos = (_mapPos * _mapCtrl.GetChipSize().ToVector2Int()).ToVector2();
}

bool MapCursor::PutCheck(const Input& input, const std::string& key)
{
	_putItv--;
	if (input.GetButton(0, key))
	{
		if (_putItv <= 0)
		{
			_putItv = std::max(_putItvCurrentMax / 2, 5);
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

MapCursor::MapCursor(MapCtrl& mapCtrl):_mapCtrl(mapCtrl), _moveItvMax(30), _putItvMax(30)
{
	_pos = Vector2();
	_mapPos = Vector2Int();

	_moveItv = 0;
	_moveItvCurrentMax = _moveItvMax;

	_putItv = 0;
	_putItvCurrentMax = _putItvMax;
}

MapCursor::~MapCursor()
{
}

Vector2Int MapCursor::GetMapPos() const
{
	return _mapPos;
}
