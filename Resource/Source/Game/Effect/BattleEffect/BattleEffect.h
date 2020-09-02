#pragma once
#include "../Effect.h"
#include <vector>

class BattleCharactor;

class BattleEffect :
	public Effect
{
protected:
	std::vector<std::shared_ptr<Effect>>& _effects;
	BattleCharactor& _self;
	BattleCharactor& _target; 
	const bool _critical;

	unsigned int AddDamage();

public:
	BattleEffect(BattleCharactor& self, BattleCharactor& target, std::vector<std::shared_ptr<Effect>>& effects, 
		bool critical, Camera& camera, bool cameraActive = false);
	~BattleEffect();

	virtual void Update(const Input& input)override;
	virtual void Draw()override;
};

