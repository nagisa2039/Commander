#pragma once
#include "../Utility/Geometry.h"
#include <memory>
#include <deque>

class Actor;

class Camera
{
private:
	Rect _rect;
	std::deque<Actor*> _targets;

	Vector2 _pos;
	Rect _limitRect;

	void NormalFollow();
	void LooseFollow();

	void(Camera::* _follower)();

public:
	Camera(const Rect& rect);
	~Camera();

	void Update();

	void AddTargetActor(Actor* target);
	void RemoveTargetActor(Actor* target);

	void PopTargetActor();
	void ClearTargetActor();

	Vector2Int GetCameraOffset()const;
	void SetPos(const Vector3& pos);
	void SetLimitRect(const Rect& rect);

	void SetLooseFollow(bool lose);

	const Rect& GetRect()const;
};

