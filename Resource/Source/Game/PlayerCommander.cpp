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
	// 選択中のキャラが移動中なら選択を無効にする
	if (_selectChar != nullptr && _selectChar->GetIsMoveAnim())return;

	// キャラクターが移動終了したらルートサーチする
	if (_routSearch)
	{
		for (auto& charactor : _charactors)
		{
			charactor->RouteSearch();
		}
		_routSearch = false;
	}

	// 選択しているキャラが行動不可なら選択を強制解除する
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
			// 行動可能な自軍?
			if (charactor->GetTeam() == _ctrlTeam && charactor->GetCanMove())
			{
				auto SetSelectChar = [&](Charactor* charactor)
				{
					_selectChar = &*charactor;
					_selectChar->SetIsSelect(true);
				};

				// 未選択ならそのキャラを選択中にする
				if (_selectChar == nullptr)
				{
					SetSelectChar(charactor);
					return;
				}

				// 選択中のキャラではないか
				if (_selectChar != &*charactor)
				{
					_selectChar->SetIsSelect(false);
					SetSelectChar(charactor);
				}
				else
				{
					// 選択中のキャラを行動終了にする
					charactor->MoveEnd();
					_routSearch = true;
				}
				return;
			}

			// 戦闘を行う	選択中のキャラがいるなら移動
			if (_selectChar != nullptr && _selectChar->GetCanMove())
			{
				_selectChar->MoveMapPos(_mapPos);
				_routSearch = true;
				return;
			}
		}
		else
		{
			// 選択中のキャラがいるなら移動
			if (_selectChar != nullptr && _selectChar->GetCanMove())
			{
				_selectChar->MoveMapPos(_mapPos);
				_routSearch = true;
				return;
			}

			// メニューを開く
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
