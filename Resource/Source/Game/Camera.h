#pragma once
#include "../Utility/Geometry.h"
#include <vector>
#include <memory>

class Actor;

class Camera
{
private:
	Rect _rect;
	std::vector<std::shared_ptr<Actor>> _targets;

	Vector3 _pos;
	Rect _limitRect;

public:
	Camera(const Rect& rect);
	~Camera();

	void Update();

	void AddTargetActor(std::shared_ptr<Actor> target);
	void RemoveTargetActor(std::shared_ptr<Actor> target);

	Vector2Int GetCameraOffset()const;
	void SetLimitRect(const Rect& rect);
};

