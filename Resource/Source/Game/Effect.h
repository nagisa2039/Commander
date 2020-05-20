#pragma once
#include "Actor.h"
#include <memory>
#include "Geometry.h"

class Animator;

class Effect :
	public Actor
{
protected:
	std::shared_ptr<Animator> _animator;
	Size _size;
	bool _cameraActive;

public:
	Effect(const Vector2Int& pos, Camera& camera, bool cameraActive = false);
	~Effect();

	virtual void Update(const Input& input) override;
	virtual void Draw() override;

	virtual void Reset();
};

