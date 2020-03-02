#include "PlayerCursor.h"
#include "../Utility/DxLibUtility.h"
#include "Camera.h"
#include "../Utility/Input.h"
#include "Charactor.h"
#include "MapCtrl.h"
#include <DxLib.h>
#include "Animator.h"
#include "Application.h"
#include "FileSystem.h"

using namespace std;

PlayerCursor::PlayerCursor(std::vector<std::shared_ptr<Charactor>>& charactors, MapCtrl& mapCtrl, const Team ctrlTeam)
	:_charactors(charactors), _ctrlTeam(ctrlTeam), MapCursor(mapCtrl)
{
	_selectChar = nullptr;
	_pos = Vector2();
	_animCnt = 0;
}

PlayerCursor::~PlayerCursor()
{
}

void PlayerCursor::Update(const Input& input)
{
	CursorMove(input);

	CharactorControl(input);
	_animCnt++;
}

void PlayerCursor::CharactorControl(const Input& input)
{
	// �I�𒆂̃L�������ړ����Ȃ�I���𖳌��ɂ���
	if (_selectChar != nullptr && _selectChar->GetIsMoveAnim())
	{
		return;
	}

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

		if (_selectChar != nullptr && _selectChar->GetCanMove())
		{
			_selectChar->MoveMapPos(_mapPos);
			return;
		}
	}
}

void PlayerCursor::Draw(const Camera& camera)
{
	auto chipSize = _mapCtrl.GetChipSize();
	auto offset = camera.GetCameraOffset();
	auto handle = Application::Instance().GetFileSystem()->GetImageHandle("Resource/Image/MapChip/cursor.png");
	Size graphSize;
	GetGraphSize(handle, graphSize);
	
	DrawRectRotaGraph(offset + _mapPos * chipSize + chipSize*0.5, Vector2Int(0, 0), graphSize, 
		((_animCnt/30)%2 ? 0.8f : 1.0f) * chipSize.w / graphSize.w, 0.0f, handle);
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
