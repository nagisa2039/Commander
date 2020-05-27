#pragma once
#include "Effect.h"
#include "TimeLine.h"
#include <memory>

class MissEffect :
	public Effect
{
private:
	std::unique_ptr<Track<float>> _animPosYTrack;

public:
	MissEffect(const Vector2Int& pos, Camera& camera);
	~MissEffect();

	void Update(const Input& input)override;
	void Draw()override;
};

