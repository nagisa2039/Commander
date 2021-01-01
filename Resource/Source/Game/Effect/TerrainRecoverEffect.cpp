#include "TerrainRecoverEffect.h"
#include "Animator.h"
#include <DxLib.h>

TerrainRecoverEffect::TerrainRecoverEffect(const Vector2Int& pos, Camera& camera) :Effect(pos, camera, true)
{
	Vector2Int animCnt{ 10, 1 };
	_animator->SetImage("Resource/Image/Effect/terrainRecoverEffect.png");
	_animator->AddAnimDiv("normal", animCnt, 2, false);
	_animator->ChangeAnim("normal");
	_size = _animator->GetImageSize() / animCnt;
}

TerrainRecoverEffect::~TerrainRecoverEffect()
{
}