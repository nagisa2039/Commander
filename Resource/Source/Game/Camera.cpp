#include "Camera.h"
#include "Actor.h"
#include <algorithm>
#include "../System/Application.h"

using namespace std;

Camera::Camera(const Rect& rect) :_rect(rect)
{
	_pos = Vector3(0,0,0);

	// size.w == 0 ÇÕêßå¿Ç»Çµ
	_limitRect.size.w = 0;
}

Camera::~Camera()
{
}

void Camera::Update()
{
	if (_targets.size() > 0)
	{
		// ÉJÉÅÉâÇÃèÍèäÇåàÇﬂÇÈ
		Vector2 pos2D = { 0.0f, 0.0f };
		for (auto& target : _targets)
		{
			pos2D += target->GetCenterPos();
		}
		Vector3 targetPos = Vector3(pos2D.x, pos2D.y, 0) / _targets.size();
		_pos = Lerp(_pos, targetPos, 0.05f);
	}

	_rect.center = Vector2Int(_pos.x, _pos.y);

	// size.w == 0 ÇÕêßå¿Ç»Çµ
	if (_limitRect.Width() != 0)
	{
		// îÕàÕêßå‰
		if (_rect.Left() < _limitRect.Left())
		{
			_rect.center.x = _limitRect.Left() + _rect.Width() / 2;
			_pos.x = _rect.center.x;
		}
		if (_rect.Right() > _limitRect.Right())
		{
			_rect.center.x = _limitRect.Right() - _rect.Width() / 2;
			_pos.x = _rect.center.x;
		}
		if (_rect.Top() < _limitRect.Top())
		{
			_rect.center.y = _limitRect.Top() + _rect.Height() / 2;
			_pos.y = _rect.center.y;
		}
		if (_rect.Botton() > _limitRect.Botton())
		{
			_rect.center.y = _limitRect.Botton() - _rect.Height() / 2;
			_pos.y = _rect.center.y;
		}
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

void Camera::SetPos(const Vector3& pos)
{
	_pos = pos;
}

void Camera::SetLimitRect(const Rect& rect)
{
	_limitRect = rect;
}
