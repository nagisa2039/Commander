#pragma once
#include "TimeLine.h"
#include "FlyText.h"
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

class PopupMissEffect :
	public PopupText
{
public:
	PopupMissEffect(const Vector2Int& pos,
		Camera& camera, bool cameraActive, bool critical = false);
	~PopupMissEffect() = default;
};