#include "Swordsman.h"
#include "MapCtrl.h"
#include "../Utility/DxLibUtility.h"
#include "Camera.h"
#include "Animator.h"

Swordsman::Swordsman(const Vector2Int& mapPos, const Team team, MapCtrl& mapCtrl) :Charactor(mapPos, team, mapCtrl)
{
	const Size divSize = Size(32,32);
	_animator->SetImage("Resource/Image/Charactor/charactor.png");

	_animator->AddAnimDiv("DownWalk",	Rect(Vector2Int(16, 16 + 32 * 0), divSize), 2, 30, true, false);
	_animator->AddAnimDiv("LeftWalk",	Rect(Vector2Int(16, 16 + 32 * 1), divSize), 2, 30, true, false);
	_animator->AddAnimDiv("RightWalk",	Rect(Vector2Int(16, 16 + 32 * 2), divSize), 2, 30, true, false);
	_animator->AddAnimDiv("UpWalk",		Rect(Vector2Int(16, 16 + 32 * 3), divSize), 2, 30, true, false);
	_animator->ChangeAnim("LeftWalk");

	_dirTable[Dir::left].animName = "LeftWalk";
	_dirTable[Dir::right].animName = "RightWalk";
	_dirTable[Dir::up].animName = "UpWalk";
	_dirTable[Dir::down].animName = "DownWalk";

	_dir = Dir::down;

	_status.move = 5;
	_startStatus = _status;
}

Swordsman::~Swordsman()
{
}

void Swordsman::Update(const Input& input)
{
	Move();

	_animator->ChangeAnim(_dirTable[_dir].animName);
	_animator->Update();
}

void Swordsman::Draw(const Camera& camera)
{
	auto offset = camera.GetCameraOffset();
	auto chipSize = _mapCtrl.GetChipSize().ToVector2Int();

	DrawMovableMass(camera);

	auto imgSize = _animator->GetAnimRect().size.ToVector2Int();
	_animator->Draw(offset + _pos.ToVector2Int() - imgSize + chipSize);

	auto circleOffset = Vector2Int(0, -chipSize.y/2);
	DrawCircle(circleOffset + offset + _pos.ToVector2Int() + chipSize * 0.5, chipSize.x / 4, GetTeamColor(), true);
}
