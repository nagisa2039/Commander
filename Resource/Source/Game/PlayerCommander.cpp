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

using namespace std;

PlayerCommander::PlayerCommander(std::vector<std::shared_ptr<Charactor>>& charactors, MapCtrl& mapCtrl, const Team ctrlTeam):
	Commander(charactors, mapCtrl, ctrlTeam)
{
	_playerUI = make_unique<PlayerUI>(*this, mapCtrl);
}

PlayerCommander::~PlayerCommander()
{
}

void PlayerCommander::CharactorControl(const Input& input)
{
	// 硬直
	_rigid = max(_rigid - 1, 0);
	if (_rigid > 0)  return;

	// 選択中のキャラが移動中なら選択を無効にする
	if (_selectChar != nullptr && _selectChar->GetIsMoveAnim())
	{
		return;
	}

	if (input.GetButtonDown(0, "space"))
	{
		// そのマスにユニットがいないか探す
		for (auto& charactor : _charactors)
		{
			if (_mapPos == charactor->GetMapPos())
			{
				// 自軍?
				if (charactor->GetTeam() == _ctrlTeam)
				{
					auto SetSelectChar = [&](std::shared_ptr<Charactor> charactor)
					{
						_selectChar = &*charactor;
						_selectChar->SetIsSelect(true);
						_selectChar->RouteSearch();
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
					}
					return;
				}
				else
				{
				}
			}
		}

		// 選択中のキャラがいるなら移動
		if (_selectChar != nullptr && _selectChar->GetCanMove())
		{
			_selectChar->MoveMapPos(_mapPos);
			return;
		}

		// メニューを開く
		_playerUI->OpenMenu();
	}
}

void PlayerCommander::Update(const Input& input)
{
	_playerUI->Update(input);

	if (_end) return;

	if (_playerUI->GetMenuIsOpen()) return;

	CursorMove(input);

	CharactorControl(input);

	ChaeckCharactor();

	_animCnt++;
}

void PlayerCommander::Draw(const Camera& camera)
{
	auto chipSize = _mapCtrl.GetChipSize();
	auto offset = camera.GetCameraOffset();
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
