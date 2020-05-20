#pragma once
#include "../Utility/Geometry.h"

class Input;
class Camera;

class Actor
{
protected:
	Vector2 _pos;
	bool _delete;
	Camera& _camera;

public:
	Actor(Camera& camera);
	~Actor();

	virtual void Update(const Input& input) = 0;
	virtual void Draw() = 0;

	Vector2 GetActorPos()const;
	virtual Vector2 GetCenterPos()const;
	bool GetDelete()const;

	void SetPos(const Vector2& pos);
};

