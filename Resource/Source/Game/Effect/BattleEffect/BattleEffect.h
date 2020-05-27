#pragma once
#include "../Effect.h"
#include <vector>

class BattleEffect :
	public Effect
{
private:
	Vector2Int _targetPos;
	std::vector<std::shared_ptr<Effect>>& _effects;

public:
	BattleEffect(const Vector2Int& pos, const Vector2Int targetPos, std::vector<std::shared_ptr<Effect>>& effects, Camera& camera, bool cameraActive = false);
	~BattleEffect();

	virtual void Update(const Input& input)override;
};

