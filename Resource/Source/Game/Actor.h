#pragma once
#include "../Utility/Geometry.h"

class Input;
class Camera;

class Actor
{
protected:
	Vector2 _pos;
	bool _delete;

public:
	Actor();
	~Actor();

	virtual void Update(const Input& input) = 0;
	virtual void Draw(const Camera& camera) = 0;

	Vector2 GetActorPos()const;
	bool GetDelete()const;
};

