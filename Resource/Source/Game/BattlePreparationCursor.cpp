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
		auto charactor = _mapCtrl.GetMapPosChar(_mapPos);
		if (charactor != nullptr && charactor->GetTeam() != Team::player)return;

		if (_selectChar == nullptr)
		{
			_selectChar = charactor;
		}
		else
		{
			if (_selectChar == charactor)return;

			charactor->InitmapPos(_selectChar->GetMapPos());
			_selectChar->InitmapPos(_mapPos);
			_selectChar = nullptr;
		}
	}

	if (input.GetButtonDown(0, "ok"))
	{
		_end = true;
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
	for (const auto& charactorChip : _mapCtrl.GetCharactorChips())
	{
		if (charactorChip.team != Team::player)continue;

		if (_selectChar != nullptr && charactorChip.mapPos == _selectChar->GetMapPos())
		{
			_mapCtrl.DrawSortieMass(offset, charactorChip, 0xffff00, 0xffff00);
		}
		else
		{
			_mapCtrl.DrawSortieMass(offset, charactorChip);
		}
	}
}

bool BattlePreparationCursor::GetEnd() const
{
	return _end;
}
