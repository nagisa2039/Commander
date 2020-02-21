#include "EditCursor.h"
#include "../Utility/Input.h"
#include "MapCtrl.h"
#include "../Utility/DxLibUtility.h"
#include "Camera.h"

EditCursor::EditCursor(MapCtrl& mapCtrl): _mapCrtl(mapCtrl)
{
	_mapPos = Vector2Int(0, 0);
	_pos = Vector2(0, 0);
	_mapChip = Forest;
	_animCnt = 0;
}

EditCursor::~EditCursor()
{
}

void EditCursor::Update(const Input& input)
{
	auto Move = [&](const std::string& key, const Vector2Int& move)
	{
		auto mapSize = _mapCrtl.GetMapSize();
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

	Move("up",		Vector2Int(0,-1));
	Move("down",	Vector2Int(0, 1));
	Move("left",	Vector2Int(-1, 0));
	Move("right",	Vector2Int(1, 0));

	auto ChipChange = [&](const char key, const int move)
	{
		if (input.GetButtonDown(key))
		{
			_mapChip = static_cast<Map_Chip>(_mapChip + move);
			if (_mapChip < None)
			{
				_mapChip = River_Corner3;
			}
			if (_mapChip > River_Corner3)
			{
				_mapChip = None;
			}
			_animCnt = 0;
		}
	};

	ChipChange(KEY_INPUT_Z,  1);
	ChipChange(KEY_INPUT_X, -1);

	if (input.GetButtonDown(0, "space"))
	{
		_mapCrtl.SetMapChip(_mapPos, _mapChip);
	}

	_animCnt+=5;
	_pos = (_mapPos * _mapCrtl.GetChipSize().ToIntVector()).ToFloatVector();
}

void EditCursor::Draw(const Camera& camera)
{
	auto offset = camera.GetCameraOffset();

	int alpha = abs(255 - (_animCnt % 512));

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
	if (_mapChip != None)
	{
		_mapCrtl.DrawMapChip(_mapPos, _mapChip, offset);
	}
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 - alpha);
	auto chipSize = _mapCrtl.GetChipSize().ToIntVector();
	DrawBox(offset + _mapPos * chipSize, offset + (_mapPos +1) * chipSize, 0xffffff, true);

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
}
