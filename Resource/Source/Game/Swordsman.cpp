#include "Swordsman.h"
#include "MapCtrl.h"
#include "../Utility/DxLibUtility.h"
#include "Camera.h"

Swordsman::Swordsman(const Vector2Int& mapPos, const Team team, MapCtrl& mapCtrl):Charactor(mapPos, team, mapCtrl)
{
}

Swordsman::~Swordsman()
{
}

void Swordsman::Update(const Input& input)
{
	Move();
}

void Swordsman::Draw(const Camera& camera)
{
	auto offset = camera.GetCameraOffset();
	auto chipSize = _mapCtrl.GetChipSize().ToVector2Int();

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
	for (const auto& movePos : _resutlPosList)
	{
		DrawBox(movePos.mapPos * chipSize, (movePos.mapPos+1) * chipSize, movePos.attack ? 0xff0000 : 0x0000ff);
	}
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);

	DrawCircle(offset + _pos.ToVector2Int() + chipSize * 0.5, chipSize.x / 3, 0x00ff00, true);
}
