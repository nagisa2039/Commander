#include "PlayerCursor.h"
#include "../Utility/DxLibUtility.h"
#include "Camera.h"
#include "../Utility/Input.h"
#include "Charactor.h"
#include "MapCtrl.h"

PlayerCursor::PlayerCursor(std::vector<std::shared_ptr<Charactor>>& charactors, MapCtrl& mapCtrl, const Team ctrlTeam)
	:_charactors(charactors), _ctrlTeam(ctrlTeam), MapCursor(mapCtrl)
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
		// ���̃}�X�Ƀ��j�b�g�����Ȃ����T��
		for (auto& charactor : _charactors)
		{
			if (_mapPos == charactor->GetMapPos()
				&& charactor->GetTeam() == _ctrlTeam)
			{
				auto SetSelectChar = [&](std::shared_ptr<Charactor> charactor)
				{
					_selectChar = &*charactor;
					_selectChar->SetIsSelect(true);
					_selectChar->RouteSearch();
				};

				// ���I���Ȃ炻�̃L������I�𒆂ɂ���
				if (_selectChar == nullptr )
				{
					SetSelectChar(charactor);
					return;
				}

				// �I�𒆂̃L�����ł͂Ȃ���
				if (_selectChar != &*charactor)
				{
					_selectChar->SetIsSelect(false);
					SetSelectChar(charactor);
				}
				else
				{
					// �I�𒆂̃L�������s���I���ɂ���
					charactor->MoveEnd();
				}
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

void PlayerCursor::TurnReset()
{
	if (_selectChar == nullptr)
	{
		return;
	}
	_selectChar->SetIsSelect(false);
	_selectChar = nullptr;
}
