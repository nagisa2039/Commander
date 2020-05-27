#include "BattleEffect.h"

BattleEffect::BattleEffect(const Vector2Int& pos, const Vector2Int targetPos, std::vector<std::shared_ptr<Effect>>& effects, Camera& camera, bool cameraActive)
	: Effect(pos, camera, cameraActive), _targetPos(targetPos), _effects(effects)
{
}

BattleEffect::~BattleEffect()
{
}

void BattleEffect::Update(const Input& input)
{
	Effect::Update(input);
}

void BattleEffect::Draw()
{
	Effect::Draw();
}
