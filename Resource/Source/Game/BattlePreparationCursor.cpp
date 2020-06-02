#include "BattlePreparationCursor.h"
#include "Input.h"
#include "Camera.h"
#include "MapCtrl.h"
#include "Application.h"
#include "Charactor.h"
#include "FileSystem.h"
#include "DxLibUtility.h"
#include "UI/PreparationUI.h"

BattlePreparationCursor::BattlePreparationCursor(MapCtrl& mapCtrl, Camera& camera):MapCursor(mapCtrl, camera)
{
	_selectChar = nullptr;
	_end = false;

	_exRateTrack = std::make_unique<Track<float>>(true);
	_exRateTrack->AddKey(0, 0.8f);
	_exRateTrack->AddKey(30, 1.0f);
	_exRateTrack->AddKey(60, 0.8f);
}

BattlePreparationCursor::~BattlePreparationCursor()
{
}

void BattlePreparationCursor::Update(const Input& input)
{
	CursorMove(input);
	_exRateTrack->Update();

	if (input.GetButtonDown(0, "ok") || input.GetButtonDown(1, "ok"))
	{
		Select();
	}

	if (input.GetButtonDown(0, "back") || input.GetButtonDown(1, "back"))
	{
		_end = true;
	}
}

void BattlePreparationCursor::Select()
{
	auto charactorChips = _mapCtrl.GetCharactorChipInf(_mapPos);
	auto charactor = _mapCtrl.GetMapPosChar(_mapPos);

	if(charactorChips.team != Team::player || charactorChips.type == CharactorType::max)return;

	if (_selectChar == nullptr)
	{
		if (charactor == nullptr)return;

		_selectChar = charactor;
		return;
	}

	if (_selectChar == charactor)
	{
		_selectChar = nullptr;
		return;
	}

	if (charactor == nullptr)
	{
		if (charactorChips.team != Team::player)return;

		_selectChar->InitmapPos(charactorChips.mapPos);
	}
	else
	{
		charactor->InitmapPos(_selectChar->GetMapPos());
		_selectChar->InitmapPos(_mapPos);
	}
	_selectChar = nullptr;
}

void BattlePreparationCursor::Draw()
{
	auto chipSize = _mapCtrl.GetChipSize();
	auto offset = _camera.GetCameraOffset();
	
	auto handle = Application::Instance().GetFileSystem()->GetImageHandle("Resource/Image/MapChip/cursor.png");
	Size graphSize;
	GetGraphSize(handle, graphSize);

	DrawRectRotaGraph(offset + _mapPos * chipSize + chipSize * 0.5, Vector2Int(0, 0), graphSize,
		_exRateTrack->GetValue() * (chipSize.w / static_cast<float>(graphSize.w)), 0.0f, handle);
}

void BattlePreparationCursor::DrawsSortieMass()
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
