#pragma once
#include "BattleEffect.h"
#include "Dir.h"
class ArrowEffect :
	public BattleEffect
{
private:
	Dir _dir;

public:
	ArrowEffect(const Vector2Int& pos, const Vector2Int& targetPos, std::vector<std::shared_ptr<Effect>>& effects, Camera& camera);
	~ArrowEffect();

	void Update(const Input& input)override;
};

