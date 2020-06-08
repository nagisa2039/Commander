#include "TerrainDamageEffect.h"
#include "Animator.h"
#include <Dxlib.h>

TerrainDamageEffect::TerrainDamageEffect(const Vector2Int& pos, Camera& camera):Effect(pos, camera, true)
{
	Vector2Int animCnt(5, 1);
	_animator->SetImage("Resource/Image/Effect/terrainDamageEffect.png");
	_animator->AddAnimDiv("normal", animCnt, 5, false);
	_animator->ChangeAnim("normal");
	_size = _animator->GetImageSize() / animCnt;
}

TerrainDamageEffect::~TerrainDamageEffect()
{
}

void TerrainDamageEffect::Draw()
{
	Effect::Draw();
}
