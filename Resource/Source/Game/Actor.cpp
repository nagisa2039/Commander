#include "Actor.h"

Actor::Actor()
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

bool Actor::GetDelete() const
{
	return _delete;
}
