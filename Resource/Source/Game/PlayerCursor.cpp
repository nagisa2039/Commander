#include "PlayerCursor.h"
#include "../Utility/DxLibUtility.h"
#include "Camera.h"
#include "../Utility/Input.h"
#include "Charactor.h"
#include "MapCtrl.h"

PlayerCursor::PlayerCursor(std::vector<std::shared_ptr<Charactor>>& charactors, MapCtrl& mapCtrl)
	:_charactors(charactors), MapCursor(mapCtrl)
{
	_charIdx = -1;
}

PlayerCursor::~PlayerCursor()
{
}

void PlayerCursor::Update(const Input& input)
{
	CursorMove(input);

	if (input.GetButtonDown(0, "space"))
	{
		if (_charIdx >= 0)
		{
			_charactors[_charIdx]->MoveMapPos(_mapPos);
		}
		else
		{
			for (int idx = 0; idx < _charactors.size(); idx++)
			{
				if (_mapPos == _charactors[idx]->GetMapPos())
				{
					_charIdx = idx;
					break;
				}
			}
		}
	}
}

void PlayerCursor::Draw(const Camera& camera)
{
	auto chipSize = _mapCtrl.GetChipSize().ToVector2Int();
	auto offset = camera.GetCameraOffset();
	DrawBox(offset + _pos.ToVector2Int(), offset + _pos.ToVector2Int() + chipSize, 0xaaaaaa, false);
}

Vector2Int PlayerCursor::GetMapPos() const
{
	return _mapPos;
}
