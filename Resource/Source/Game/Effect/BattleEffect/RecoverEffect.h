#pragma once
#include "BattleEffect.h"
class RecoverEffect :
	public BattleEffect
{
public:
	RecoverEffect(BattleCharactor& self, BattleCharactor& target, std::vector<std::shared_ptr<Effect>>& effects, Camera& camera);
	~RecoverEffect();
};

