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
	// ÉJÉÅÉâÇÃèÍèäÇåàÇﬂÇÈ
	Vector2 pos2D = {0.0f, 0.0f};
	for (auto& target : _targets)
	{
		pos2D += target->GetActorPos();
	}
	Vector3 targetPos = Vector3(pos2D.x, pos2D.y, 0) / _targets.size();
	targetPos = Lerp(_pos, targetPos, 0.2f);

	_rect.center = Vector2Int(targetPos.x, targetPos.y);

	// îÕàÕêßå‰
	if (_rect.center.x < _rect.Width() / 2)
	{
		_rect.center.x = _rect.Width() / 2;
	}
	if (_rect.center.y < _rect.Height() / 2)
	{
		_rect.center.y = _rect.Height() / 2;
	}
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
