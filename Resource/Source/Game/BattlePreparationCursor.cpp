#include "BattlePreparationCursor.h"
#include "Input.h"
#include "Camera.h"
#include "MapCtrl.h"
#include "Application.h"
#include "Charactor.h"
#include "FileSystem.h"
#include "DxLibUtility.h"
#include "UI/PreparationUI.h"
#include "UI/TerrainInf.h"
#include "UI/StatusInfomation.h"
#include "UI/StatusWindow/StatusWindow.h"

using namespace std;

BattlePreparationCursor::BattlePreparationCursor(MapCtrl& mapCtrl, Camera& camera):MapCursor(mapCtrl, camera)
{
	_selectChar = nullptr;
	_end = false;

	_exRateTrack = std::make_unique<Track<float>>(true);
	_exRateTrack->AddKey(0, 0.8f);
	_exRateTrack->AddKey(30, 1.0f);
	_exRateTrack->AddKey(60, 0.8f);

	_terrainInf = make_shared<TerrainInf>(nullptr, _mapCtrl, _mapPos);
	_statusInf = make_shared<StatusInf>(nullptr, _mapCtrl, _mapPos);
	_statusWindow.reset();
}

BattlePreparationCursor::~BattlePreparationCursor()
{
}

void BattlePreparationCursor::Update(const Input& input)
{
	_exRateTrack->Update();
	if (_statusWindow)
	{
		_terrainInf->Close();
		_statusInf->Close();

		if (_statusWindow->GetDelete())
		{
			_statusWindow.reset();
		}
	}
	else
	{
		CursorMove(input);

		if (input.GetButtonDown(0, "ok") || input.GetButtonDown(1, "ok") || input.GetButtonDown(0, "mouseLeft"))
		{
			Select();
		}

		if (input.GetButtonDown(0, "back") || input.GetButtonDown(1, "back") || input.GetButtonDown(0, "mouseRight"))
		{
			_end = true;
		}

		if (input.GetButtonDown(0, "status") || input.GetButtonDown(1, "status"))
		{
			auto charactor = _mapCtrl.GetMapPosChar(_mapPos);
			if (charactor == nullptr)return;
			_statusWindow = make_shared<StatusWindow>(nullptr, *charactor);
		}

		_terrainInf->Open();
		_statusInf->Open();
	}

	auto UIUpdate = [&input](std::shared_ptr<UI> ui)
	{
		if (!ui)return;
		ui->Update(input);
	};

	UIUpdate(_statusWindow);
	UIUpdate(_terrainInf);
	UIUpdate(_statusInf);
}

void BattlePreparationCursor::Select()
{
	auto charactorChips = _mapCtrl.GetCharactorChipInf(_mapPos);
	auto charactor = _mapCtrl.GetMapPosChar(_mapPos);

	if(charactorChips.team != Team::player || charactorChips.type == CharactorType::max)return;

	// キャラクターの選択
	if (_selectChar == nullptr)
	{
		if (charactor == nullptr)return;

		_selectChar = charactor;
		return;
	}

	// 選択解除
	if (_selectChar == charactor)
	{
		_selectChar = nullptr;
		return;
	}

	// 空きマスなら
	if (charactor == nullptr)
	{
		if (charactorChips.team != Team::player)return;

		// 選択キャラをセット
		_selectChar->InitmapPos(charactorChips.mapPos);
	}
	else
	{
		// 選択キャラをそのマスにいるキャラと入れ替える
		charactor->InitmapPos(_selectChar->GetMapPos());
		_selectChar->InitmapPos(_mapPos);
	}

	// 移動したのでルートの再検索
	for (auto& charactor : _mapCtrl.GetCharacots())
	{
		charactor->RouteSearch();
	}
	_selectChar = nullptr;
}

void BattlePreparationCursor::Draw()
{
	auto chipSize = _mapCtrl.GetChipSize();
	auto offset = _camera.GetCameraOffset();
	
	auto handle = Application::Instance().GetFileSystem().GetImageHandle("Resource/Image/MapChip/cursor.png");
	Size graphSize;
	GetGraphSize(handle, graphSize);

	DrawRectRotaGraph(offset + _mapPos * chipSize + chipSize * 0.5, Vector2Int(0, 0), graphSize,
		_exRateTrack->GetValue() * (chipSize.w / static_cast<float>(graphSize.w)), 0.0f, handle);

	auto dequeDraw = [](std::shared_ptr<UI> ui)
	{
		if (ui)
		{
			ui->Draw();
		}
	};

	dequeDraw(_terrainInf);
	dequeDraw(_statusInf);
	dequeDraw(_statusWindow);
}

void BattlePreparationCursor::DrawSortieMass()
{
	auto offset = _camera.GetCameraOffset();
	for (const auto& mapDataVec : _mapCtrl.GetMapData())
	{
		for (const auto& mapData : mapDataVec)
		{
			if (mapData.charactorChip.team != Team::player)continue;

			if (_selectChar != nullptr && mapData.charactorChip.mapPos == _selectChar->GetMapPos())
			{
				_mapCtrl.DrawSortieMass(offset, mapData.charactorChip, 0xffff00, 0xffff00);
			}
			else
			{
				_mapCtrl.DrawSortieMass(offset, mapData.charactorChip);
			}
		}
	}
}

void BattlePreparationCursor::DrawMovableMass()
{
	auto charactor = _mapCtrl.GetMapPosChar(_mapPos);
	if (charactor == nullptr) return;

	charactor->DrawMovableMass(128);
}

bool BattlePreparationCursor::GetEnd() const
{
	return _end;
}

void BattlePreparationCursor::Start()
{
	_end = false;
	_camera.AddTargetActor(this);
	SetCursorStartPosition();
}

void BattlePreparationCursor::SetCursorStartPosition()
{
	for (const auto& charactor : _mapCtrl.GetCharacots())
	{
		if (charactor->GetTeam() == Team::player)
		{
			_mapPos = charactor->GetMapPos();
			_camera.SetPos(charactor->GetCenterPos());
			break;
		}
	}
}
