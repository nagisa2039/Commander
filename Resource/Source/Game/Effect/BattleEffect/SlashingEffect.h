#pragma once
#include "BattleEffect.h"
class SlashingEffect :
	public BattleEffect
{
public:
	SlashingEffect(BattleCharactor& self, BattleCharactor& target, std::vector<std::shared_ptr<Effect>>& effects, Camera& camera);
	~SlashingEffect();
};

