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

void PlayerCommander::NormalUpdate(const Input& input)
{
	CursorMove(input);

	if (input.GetButtonDown(0, "space"))
	{
		auto charactor = _mapCtrl.GetMapPosChar(_mapPos);
		if (charactor != nullptr)
		{
			// �s���\�Ȏ��R?
			if (charactor->GetTeam() == _ctrlTeam && charactor->GetCanMove())
			{
				// ���I���Ȃ炻�̃L������I�𒆂ɂ���
				if (_selectChar == nullptr)
				{
					SelectCharactor(charactor);
				}
			}
			return;
		}

		// ���j���[���J��
		_playerUI->OpenMenu();
		return;
	}
}

void PlayerCommander::SelectUpdate(const Input& input)
{
	CursorMove(input);

	// �I�𒆂̃L�������ړ����Ȃ�I���𖳌��ɂ���
	if (_selectChar->GetIsMoveAnim())return;

	// �I�����Ă���L�������s���s�Ȃ�I����������������
	if ( !_selectChar->GetCanMove())
	{
		SelectCharactor(nullptr);

		return;
	}

	if (input.GetButtonDown(0, "space"))
	{
		auto charactor = _mapCtrl.GetMapPosChar(_mapPos);
		if (charactor != nullptr)
		{
			if (_selectChar == charactor)
			{
				// �I�𒆂̃L�������s���I���ɂ���
				_selectChar->MoveEnd();
				//_camera.AddTargetActor(this);
				SelectCharactor(nullptr);
				return;
			}

			// �����Ȃ炻�̃L�����N�^�[��I������
			if (!_selectChar->GetStatus().heal && charactor->GetTeam() == _selectChar->GetTeam())
			{
				SelectCharactor(charactor);
				return;
			}

			if (CheckAttackMass())
			{
				// �퓬���s��
				_uniqueUpdater = &PlayerCommander::BattlePredictionUpdate;
				_playerUI->AddBattlePre();
				return;
			}
			else
			{
				// �s���\�Ȏ��R?
				if (charactor->GetTeam() == _ctrlTeam && charactor->GetCanMove())
				{
					SelectCharactor(charactor);
					return;
				}
			}
		}
		else
		{
			// �ړ��͈͓���
			if (!CheckMoveRange())return;

			// �I�𒆂̃L����������Ȃ�ړ�
			_camera.AddTargetActor(_selectChar);
			_selectChar->MoveMapPos(_mapPos);
			return;
		}
	}

	if (input.GetButtonDown(0, "back"))
	{
		if (_selectChar->GetMoved())
		{
			_selectChar->MoveCancel();
			return;
		}
		SelectCharactor(nullptr);
		return;
	}
}

bool PlayerCommander::CheckMoveRange()
{
	if (_selectChar == nullptr) return false;

	return (_selectChar->GetResutlPosListVec2()[_mapPos.y][_mapPos.x].size() > 0);
}

bool PlayerCommander::CheckAttackMass()
{
	if (_selectChar == nullptr) return false;

	auto resultPosList = _selectChar->GetResutlPosListVec2()[_mapPos.y][_mapPos.x];
	if (resultPosList.size() <= 0) return false; 

	for (const auto& resultPos : resultPosList)
	{
		if (resultPos.attack)return true;
	}
	return false;
}

void PlayerCommander::TerrainEffectUpdate(const Input& input)
{
	bool end = true;
	for (auto& charactor : _charactors)
	{
		if (charactor->GetTeam() != _ctrlTeam)continue;

		bool effectEnd = charactor->GetTerrainEffectEnd();
		end = end && effectEnd;
	}

	if (end)
	{
		_uniqueUpdater = &PlayerCommander::NormalUpdate;
	}
}

void PlayerCommander::BattlePredictionUpdate(const Input& input)
{
	if (input.GetButtonDown(0, "space"))
	{
		// �퓬���s��	�I�𒆂̃L����������Ȃ�ړ�
		if (_selectChar->GetCanMove())
		{
			_playerUI->ClearBattlePre();
			_camera.AddTargetActor(_selectChar);
			_selectChar->MoveMapPos(_mapPos);
			_uniqueUpdater = &PlayerCommander::BattaleUpdate;
			return;
		}
	}

	if (input.GetButtonDown(0, "back"))
	{
		_playerUI->ClearBattlePre();
		_uniqueUpdater = &PlayerCommander::SelectUpdate;
		return;
	}
}

void PlayerCommander::BattaleUpdate(const Input& input)
{
	if (!_selectChar->GetCanMove())
	{
		_uniqueUpdater = &PlayerCommander::NormalUpdate;
		SelectCharactor(nullptr);
		return;
	}
}

void PlayerCommander::SelectCharactor(Charactor* charactor)
{
	if (_selectChar != nullptr)
	{
		_selectChar->SetIsSelect(false);
	}
	if (charactor == nullptr)
	{
		_selectChar = nullptr;
		_uniqueUpdater = &PlayerCommander::NormalUpdate;

		for (const auto& charactor : _charactors)
		{
			if (charactor->GetTeam() == _ctrlTeam && charactor->GetCanMove())
			{
				_mapPos = charactor->GetMapPos();
				CursorMoveMoment();
				break;
			}
		}
	}
	else
	{
		_selectChar = &*charactor;
		_selectChar->SetIsSelect(true);
		_uniqueUpdater = &PlayerCommander::SelectUpdate;
	}
}

void PlayerCommander::CursorMoveMoment()
{
}

PlayerCommander::PlayerCommander(std::vector<std::shared_ptr<Charactor>>& charactors, MapCtrl& mapCtrl, const Team ctrlTeam, Camera& camera):
	Commander(charactors, mapCtrl, ctrlTeam, camera)
{
	_playerUI = make_unique<PlayerUI>(*this, mapCtrl);

	_uniqueUpdater = &PlayerCommander::TerrainEffectUpdate;
}

PlayerCommander::~PlayerCommander()
{
}

void PlayerCommander::Update(const Input& input)
{
	_playerUI->Update(input);

	if (_end) return;

	if (_playerUI->GetUIIsOpen()) return;

	(this->*_uniqueUpdater)(input);

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
		_selectChar->DrawRoute(_mapPos);
	}
}

void PlayerCommander::End()
{
	Commander::End();
	_playerUI->CloseMenu(false);
}
