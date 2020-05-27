#pragma once
#include "Effect.h"
#include <memory>
#include "TimeLine.h"

class CorsorTarget :
	public Effect
{
private:
	std::unique_ptr<Track<float>> _exRateTrack;
	Size _chipSize;

public:
	CorsorTarget(const Vector2Int& centerPos, Camera& camera, bool cameraActive, const Size& chipSize);
	~CorsorTarget();

	void Update(const Input& input)override;
	void Draw()override;

	void Reset()override;
};