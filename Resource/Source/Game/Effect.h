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

public:
	Effect(const Vector2Int& pos);
	~Effect();

	virtual void Update(const Input& input) override;
	virtual void Draw(const Camera& camera) override;
};

