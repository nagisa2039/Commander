#include "Swordsman.h"
#include "MapCtrl.h"
#include "../Utility/DxLibUtility.h"
#include "Camera.h"

Swordsman::Swordsman(const Vector2Int& mapPos, const Team team, MapCtrl& mapCtrl):Charactor(mapPos, team, mapCtrl)
{
	resutlPosList = _mapCtrl.RouteSearch(mapPos, 10);
}

Swordsman::~Swordsman()
{
}

void Swordsman::Update(const Input& input)
{
	_pos = (_mapPos * _mapCtrl.GetChipSize().ToIntVector()).ToFloatVector();
}

void Swordsman::Draw(const Camera& camera)
{
	auto offset = camera.GetCameraOffset();
	auto chipSize = _mapCtrl.GetChipSize().ToIntVector();

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
	for (const auto& movePos : resutlPosList)
	{
		DrawBox(movePos.mapPos * chipSize, (movePos.mapPos+1) * chipSize, movePos.attack ? 0xff0000 : 0x0000ff);
	}
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);

	DrawCircle(offset + _mapPos * chipSize + chipSize * 0.5, chipSize.x / 2, 0x00ff00, true);
}
