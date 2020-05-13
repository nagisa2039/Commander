#include "PlayerCommander.h"
#include "../Utility/DxLibUtility.h"
#include "Camera.h"
#include "../Utility/Input.h"
#include "Charactor.h"
#include "MapCtrl.h"
#include <DxLib.h>
#include "Animator.h"
#include "Application.h"
#include "FileSystem.h"
#include "UI/PlayerUI.h"

#include "UI/StatusWindow.h"

using namespace std;

PlayerCommander::PlayerCommander(std::vector<std::shared_ptr<Charactor>>& charactors, MapCtrl& mapCtrl, const Team ctrlTeam, Camera& camera):
	Commander(charactors, mapCtrl, ctrlTeam, camera)
{
	_playerUI = make_unique<PlayerUI>(*this, mapCtrl);
}

PlayerCommander::~PlayerCommander()
{
}

void PlayerCommander::CharactorControl(const Input& input)
{
	// �d��
	_rigid = max(_rigid - 1, 0);
	if (_rigid > 0)  return;

	// �I�𒆂̃L�������ړ����Ȃ�I���𖳌��ɂ���
	if (_selectChar != nullptr && _selectChar->GetIsMoveAnim())return;

	if (input.GetButtonDown(0, "space"))
	{
		auto charactor = _mapCtrl.GetMapPosChar(_mapPos);
		if (charactor != nullptr)
		{
			// ���R?
			if (charactor->GetTeam() == _ctrlTeam)
			{
				auto SetSelectChar = [&](Charactor* charactor)
				{
					_selectChar = &*charactor;
					_selectChar->SetIsSelect(true);
					_selectChar->RouteSearch();
				};

				// ���I���Ȃ炻�̃L������I�𒆂ɂ���
				if (_selectChar == nullptr)
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
				return;
			}

			// �I�𒆂̃L����������Ȃ�ړ�
			if (_selectChar != nullptr && _selectChar->GetCanMove())
			{
				_selectChar->MoveMapPos(_mapPos);
				return;
			}
		}
		else
		{
			// ���j���[���J��
			_playerUI->OpenMenu();
			return;
		}
	}

}

void PlayerCommander::Update(const Input& input)
{
	_playerUI->Update(input);

	if (_end) return;

	if (_playerUI->GetUIIsOpen()) return;

	CursorMove(input);

	CharactorControl(input);

	ChaeckCharactor();

	_animCnt++;
}

void PlayerCommander::Draw()
{
	auto chipSize = _mapCtrl.GetChipSize();
	auto offset = _camera.GetCameraOffset();
	auto handle = Application::Instance().GetFileSystem()->GetImageHandle("Resource/Image/MapChip/cursor.png");
	Size graphSize;
	GetGraphSize(handle, graphSize);

	int itv = 60;
	float alpha = 1.0f - 2.0f * abs(0.5f - (_animCnt % itv) / static_cast<float>(itv));

	DrawRectRotaGraph(offset + _mapPos * chipSize + chipSize * 0.5, Vector2Int(0, 0), graphSize,
		(Lerp(0.8f, 1.0f, alpha)) * chipSize.w / graphSize.w, 0.0f, handle);

	// UI
	_playerUI->Draw();

}

void PlayerCommander::End()
{
	Commander::End();
	_playerUI->CloseMenu(false);
}
