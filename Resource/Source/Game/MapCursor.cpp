#include "MapCursor.h"
#include "MapCtrl.h"
#include "../Utility/Input.h"

void MapCursor::CursorMove(const Input& input)
{
	auto Move = [&](const std::string& key, const Vector2Int& move)
	{
		auto mapSize = _mapCtrl.GetMapCnt();
		if ((_mapPos + move).x >= 0 && (_mapPos + move).x < mapSize.w
			&& (_mapPos + move).y >= 0 && (_mapPos + move).y < mapSize.h)
		{
			if (input.GetButtonDown(0, key))
			{
				_mapPos += move;
			}
			return true;
		}
		return false;
	};

	Move("up", Vector2Int(0, -1));
	Move("down", Vector2Int(0, 1));
	Move("left", Vector2Int(-1, 0));
	Move("right", Vector2Int(1, 0));
	_pos = (_mapPos * _mapCtrl.GetChipSize().ToVector2Int()).ToVector2();
}

MapCursor::MapCursor(MapCtrl& mapCtrl):_mapCtrl(mapCtrl), _inputItvMax(30)
{
	_pos = Vector2();
	_mapPos = Vector2Int();
	_inputItv = 0;
}

MapCursor::~MapCursor()
{
}
