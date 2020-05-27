#pragma once
#include "BattleEffect.h"
#include "Dir.h"
#include "TimeLine.h"

class ArrowEffect :
	public BattleEffect
{
private:
	std::shared_ptr<Track<Vector2Int>> _moveTrack;

public:
	ArrowEffect(const Vector2Int& pos, const Vector2Int& targetPos, std::vector<std::shared_ptr<Effect>>& effects, Camera& camera);
	~ArrowEffect();

	void Update(const Input& input)override;
	void Draw()override;
};

