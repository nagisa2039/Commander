#pragma once
#include "BattleEffect.h"
class SlashingEffect :
	public BattleEffect
{
public:
	SlashingEffect(const Vector2Int& pos, const Vector2Int& targetPos, std::vector<std::shared_ptr<Effect>>& effects, Camera& camera);
	~SlashingEffect();
};

