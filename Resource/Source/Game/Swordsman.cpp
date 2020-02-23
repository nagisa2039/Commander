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

	DrawMovableMass(camera);

	DrawCircle(offset + _pos.ToVector2Int() + chipSize * 0.5, chipSize.x / 3, GetTeamColor(), true);
}
