#include "PlayerCursor.h"
#include "../Utility/DxLibUtility.h"
#include "Camera.h"
#include "../Utility/Input.h"
#include "Charactor.h"
#include "MapCtrl.h"

PlayerCursor::PlayerCursor(std::vector<std::shared_ptr<Charactor>>& charactors, MapCtrl& mapCtrl)
	:_charactors(charactors), MapCursor(mapCtrl)
{
	_selectChar = nullptr;
}

PlayerCursor::~PlayerCursor()
{
}

void PlayerCursor::Update(const Input& input)
{
	CursorMove(input);

	CharactorControl(input);
}

void PlayerCursor::CharactorControl(const Input& input)
{
	if (input.GetButtonDown(0, "space"))
	{
		for (auto& charactor : _charactors)
		{
			if (_mapPos == charactor->GetMapPos())
			{
				_selectChar = &*charactor;
				_selectChar->SetIsSelect(true);
				return;
			}
		}

		if (_selectChar != nullptr)
		{
			_selectChar->MoveMapPos(_mapPos);
			return;
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
