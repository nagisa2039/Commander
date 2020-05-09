#include "Actor.h"
#include "Camera.h"

Actor::Actor(Camera& camera) :_camera(camera)
{
	_delete = false;
}

Actor::~Actor()
{
}

Vector2 Actor::GetActorPos() const
{
	return _pos;
}

Vector2 Actor::GetCenterPos() const
{
	return _pos;
}

bool Actor::GetDelete() const
{
	return _delete;
}
