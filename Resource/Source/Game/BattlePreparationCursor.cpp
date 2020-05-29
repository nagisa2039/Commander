#include "BattlePreparationCursor.h"
#include "Input.h"
#include "Camera.h"
#include "MapCtrl.h"
#include "Application.h"
#include "Charactor.h"
#include "FileSystem.h"
#include "DxLibUtility.h"

BattlePreparationCursor::BattlePreparationCursor(MapCtrl& mapCtrl, Camera& camera):MapCursor(mapCtrl, camera)
{
	_selectChar = nullptr;
	_end = false;
}

BattlePreparationCursor::~BattlePreparationCursor()
{
}

void BattlePreparationCursor::Update(const Input& input)
{
	CursorMove(input);

	if (input.GetButtonDown(0, "space"))
	{
		Select();
	}

	if (input.GetButtonDown(0, "ok"))
	{
		_end = true;
	}
}

void BattlePreparationCursor::Select()
{
	auto charactorChips = _mapCtrl.GetCharactorChipInf(_mapPos);
	auto charactor = _mapCtrl.GetMapPosChar(_mapPos);
	if (charactor != nullptr && charactor->GetTeam() != Team::player)return;

	if (_selectChar == nullptr)
	{
		if (charactor == nullptr)return;

		_selectChar = charactor;
	}
	else
	{
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
}

void BattlePreparationCursor::Draw()
{
	auto chipSize = _mapCtrl.GetChipSize();
	auto offset = _camera.GetCameraOffset();
	auto handle = Application::Instance().GetFileSystem()->GetImageHandle("Resource/Image/MapChip/cursor.png");
	Size graphSize;
	GetGraphSize(handle, graphSize);

	DrawRectRotaGraph(offset + _mapPos * chipSize + chipSize * 0.5, Vector2Int(0, 0), graphSize,
		chipSize.w / graphSize.w, 0.0f, handle);
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
