#pragma once
#include "BattleEffect.h"
#include "TimeLine.h"

class ArrowEffect :
	public BattleEffect
{
private:
	std::shared_ptr<Track<Vector2Int>> _moveTrack;

public:
	ArrowEffect(BattleCharactor& self, BattleCharactor& target, std::vector<std::shared_ptr<Effect>>& effects, 
		bool critical, Camera& camera);
	~ArrowEffect();

	void Update(const Input& input)override;
	void Draw()override;
};

