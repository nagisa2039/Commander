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
#include "UI/MoveMenu.h"
#include "UI/StatusWindow/StatusWindow.h"
#include "SoundLoader.h"
#include "RouteManager.h"

using namespace std;

void PlayerCommander::NormalUpdate(const Input& input)
{
	CursorMove(input);

	if (input.GetButtonDown("ok"))
	{
		SoundL.PlaySE(_okH);
		auto charactor = _mapCtrl.GetMapPosChar(_mapPos);
		if (charactor != nullptr)
		{
			// 行動可能な自軍?
			if (charactor->GetTeam() == _ctrlTeam && charactor->GetCanMove())
			{
				// 未選択ならそのキャラを選択中にする
				if (_selectChar == nullptr)
				{
					SelectCharactor(charactor, true);
				}
			}
			return;
		}

		// メニューを開く
		_playerUI->OpenPlayerMenu();
		return;
	}
}

void PlayerCommander::SelectUpdate(const Input& input)
{
	CursorMove(input);

	// 選択中のキャラが移動中なら選択を無効にする
	if (_selectChar->GetIsMoveAnim())return;

	// 選択しているキャラが行動不可なら選択を強制解除する
	if ( !_selectChar->GetCanMove())
	{
		SelectCharactor(nullptr, true);

		return;
	}

	if (_selectChar->GetMoved())
	{
		auto moveMenu = _playerUI->GetMoveMenu();
		moveMenu->SetContent(_selectChar->GetRouteManager()->GetAttackPosList());
		moveMenu->Open();
	}

	auto& soundLoader = SoundL;
	if (input.GetButtonDown("ok"))
	{
		soundLoader.PlaySE(_okH);
		auto charactor = _mapCtrl.GetMapPosChar(_mapPos);
		if (charactor != nullptr)
		{
			if (_selectChar == charactor)
			{
				_selectChar->MoveDecision();
				auto moveMenu = _playerUI->GetMoveMenu();
				moveMenu->SetContent(_selectChar->GetRouteManager()->GetAttackPosList());
				moveMenu->Open();
				return;
			}

			// 指定したマスを攻撃
			AttackPrePos(_mapPos);
		}
		else
		{
			// 移動範囲内か
			if (!CheckMoveRange())return;

			// 選択中のキャラがいるなら移動
			_camera.AddTargetActor(_selectChar);
			_selectChar->MoveMapPos(_mapPos);
			return;
		}
	}

	if (input.GetButtonDown("back"))
	{
		soundLoader.PlaySE(_canselH);
		MoveCancel();
		SelectCharactor(nullptr, false);
		return;
	}
}

bool PlayerCommander::CheckMoveRange()
{
	if (_selectChar == nullptr) return false;

	return (_selectChar->GetRouteManager()->GetResutlPosListVec2()[_mapPos.y][_mapPos.x].size() > 0);
}

bool PlayerCommander::CheckAttackMass()
{
	if (_selectChar == nullptr) return false;

	auto resultPosList = _selectChar->GetRouteManager()->GetResutlPosListVec2()[_mapPos.y][_mapPos.x];
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
		_camera.ClearTargetActor();
		_camera.AddTargetActor(this);
		_uniqueUpdater = &PlayerCommander::NormalUpdate;
	}
}

void PlayerCommander::BattlePredictionUpdate(const Input& input)
{
	if (input.GetButtonDown("ok"))
	{
		// 戦闘を行う	選択中のキャラがいるなら移動
		if (_selectChar->GetCanMove())
		{
			BattleStart();
			return;
		}
	}

	if (input.GetButtonDown("back"))
	{
		BackBattalePrediction();
		return;
	}
}

void PlayerCommander::BackBattalePrediction()
{
	SoundL.PlaySE(_canselH);
	_playerUI->ClearBattlePrediction();
	_uniqueUpdater = &PlayerCommander::SelectUpdate;
}

bool PlayerCommander::MoveCancel()
{
	if (_selectChar == nullptr)return false;

	_selectChar->MoveCancel();
	_mapPos = _selectChar->GetMapPos();

	return true;
}

bool PlayerCommander::GetBackMapSelect() const
{
	return _backMapSelect;
}

void PlayerCommander::SetBackMapSelect(const bool value)
{
	_backMapSelect = value;
}

void PlayerCommander::BattaleUpdate(const Input& input)
{
	if (!_selectChar->GetCanMove())
	{
		_uniqueUpdater = &PlayerCommander::NormalUpdate;
		SelectCharactor(nullptr, true);
		return;
	}
}

void PlayerCommander::SelectCharactor(Charactor* charactor, const bool nextTarget)
{
	if (_selectChar != nullptr)
	{
		_selectChar->SetIsSelect(false);
	}
	if (charactor == nullptr)
	{
		_selectChar = nullptr;
		_uniqueUpdater = &PlayerCommander::NormalUpdate;

		if (!nextTarget)return;

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

bool PlayerCommander::AttackPrePos(const Vector2Int& mapPos)
{
	_mapPos = mapPos;
	if (CheckAttackMass())
	{
		// 戦闘を行う
		_uniqueUpdater = &PlayerCommander::BattlePredictionUpdate;
		_playerUI->OpenBattlePrediction();
		return true;
	}
	return false;
}

void PlayerCommander::BattleStart()
{
	_playerUI->ClearBattlePrediction();
	_camera.AddTargetActor(_selectChar);
	_selectChar->MoveMapPos(_mapPos);
	_uniqueUpdater = &PlayerCommander::BattaleUpdate;
	if (_selectChar->GetTeam() == _mapCtrl.GetMapPosChar(_mapPos)->GetTeam())
	{
		// 回復
		SoundL.PlaySE("Resource/Sound/SE/healStart.mp3");
	}
	else
	{
		// 攻撃
		SoundL.PlaySE("Resource/Sound/SE/battleStart.mp3");
	}
}

PlayerCommander::PlayerCommander(std::vector<std::shared_ptr<Charactor>>& charactors, MapCtrl& mapCtrl, const Team ctrlTeam, Camera& camera, const unsigned char& turnCnt):
	Commander(charactors, mapCtrl, ctrlTeam, camera)
{
	_playerUI = make_unique<PlayerUI>(*this, mapCtrl, turnCnt);

	_uniqueUpdater = &PlayerCommander::TerrainEffectUpdate;

	_backMapSelect = false;

	auto& soundLoader = SoundL;
	_okH = soundLoader.GetSoundHandle("Resource/Sound/SE/ok.mp3");
	_canselH = soundLoader.GetSoundHandle("Resource/Sound/SE/cancel.mp3");
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
	auto handle = ImageHandle("Resource/Image/MapChip/cursor.png");
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
		if (charactor == nullptr || !charactor->GetCanMove()) return;

		charactor->GetRouteManager()->DrawMovableMass(128);
	}
	else
	{
		if (_selectChar->GetIsMoveAnim())return;

		_selectChar->GetRouteManager()->DrawMovableMass(192);
		_selectChar->GetRouteManager()->DrawRoute(_mapPos);
	}
}

void PlayerCommander::StartTerrainEffectUpdate()
{
	StartTerrainEffect();
	_uniqueUpdater = &PlayerCommander::TerrainEffectUpdate;
}

void PlayerCommander::StartNormalUpdate()
{
	_uniqueUpdater = &PlayerCommander::NormalUpdate;
}

void PlayerCommander::End()
{
	Commander::End();
	_playerUI->ClosePlayerMenu(false);
}