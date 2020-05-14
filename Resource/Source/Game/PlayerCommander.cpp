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
	_routSearch = false;
}

PlayerCommander::~PlayerCommander()
{
}

void PlayerCommander::CharactorControl(const Input& input)
{
	// �I�𒆂̃L�������ړ����Ȃ�I���𖳌��ɂ���
	if (_selectChar != nullptr && _selectChar->GetIsMoveAnim())return;

	// �L�����N�^�[���ړ��I�������烋�[�g�T�[�`����
	if (_routSearch)
	{
		for (auto& charactor : _charactors)
		{
			charactor->RouteSearch();
		}
		_routSearch = false;
	}

	// �I�����Ă���L�������s���s�Ȃ�I����������������
	if (_selectChar != nullptr && !_selectChar->GetCanMove())
	{
		_selectChar = nullptr;
		bool setMapPos = false;
		for (const auto& charactor : _charactors)
		{
			if (charactor->GetTeam() == _ctrlTeam && charactor->GetCanMove() && !setMapPos)
			{
				_mapPos = charactor->GetMapPos();
				setMapPos = true;
			}
		}
	}

	if (input.GetButtonDown(0, "space"))
	{
		auto charactor = _mapCtrl.GetMapPosChar(_mapPos);
		if (charactor != nullptr)
		{
			// �s���\�Ȏ��R?
			if (charactor->GetTeam() == _ctrlTeam && charactor->GetCanMove())
			{
				auto SetSelectChar = [&](Charactor* charactor)
				{
					_selectChar = &*charactor;
					_selectChar->SetIsSelect(true);
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
					_routSearch = true;
				}
				return;
			}

			// �퓬���s��	�I�𒆂̃L����������Ȃ�ړ�
			if (_selectChar != nullptr && _selectChar->GetCanMove())
			{
				_selectChar->MoveMapPos(_mapPos);
				_routSearch = true;
				return;
			}
		}
		else
		{
			// �I�𒆂̃L����������Ȃ�ړ�
			if (_selectChar != nullptr && _selectChar->GetCanMove())
			{
				_selectChar->MoveMapPos(_mapPos);
				_routSearch = true;
				return;
			}

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

void PlayerCommander::DrawMovableMass()
{
	if (_selectChar == nullptr)
	{
		auto charactor = _mapCtrl.GetMapPosChar(_mapPos);
		if (charactor == nullptr) return;

		charactor->DrawMovableMass(92);
	}
	else
	{
		_selectChar->DrawMovableMass(128);
	}
}

void PlayerCommander::End()
{
	Commander::End();
	_playerUI->CloseMenu(false);
}
