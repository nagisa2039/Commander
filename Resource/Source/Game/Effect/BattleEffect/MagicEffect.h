#pragma once
#include "BattleEffect.h"
class MagicEffect :
	public BattleEffect
{
public:
	MagicEffect(BattleCharactor& self, BattleCharactor& target, std::vector<std::shared_ptr<Effect>>& effects, Camera& camera);
	~MagicEffect();
};

