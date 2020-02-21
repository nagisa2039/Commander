#include "Camera.h"
#include "Actor.h"
#include <algorithm>
#include "../System/Application.h"

using namespace std;

Camera::Camera(const Rect& rect) :_rect(rect)
{
}

Camera::~Camera()
{
}

void Camera::Update()
{
	Vector2 pos = {0.0f, 0.0f};
	for (auto& target : _targets)
	{
		pos += target->GetActorPos();
	}
	_rect.center = (pos / _targets.size()).ToIntVector();
}

void Camera::AddTargetActor(std::shared_ptr<Actor> target)
{
	_targets.emplace_back(target);
}

void Camera::RemoveTargetActor(std::shared_ptr<Actor> target)
{
	auto it = find(_targets.begin(), _targets.end(), target);
	_targets.erase(it);
}

Vector2Int Camera::GetCameraOffset() const
{
	auto wsize = Application::Instance().GetWindowSize();
	return Vector2Int(wsize.w/2 - _rect.center.x, wsize.h/2 - _rect.center.y);
}
