#include "Camera.h"
#include "Actor.h"
#include <algorithm>
#include "../System/Application.h"

using namespace std;

void Camera::NormalFollow()
{
	_pos = Lerp(_pos, (*_targets.begin())->GetCenterPos(), 0.1f);
}

void Camera::LooseFollow()
{
	auto targetPos = (*_targets.begin())->GetCenterPos();
	int space = 50;

	int subX = targetPos.x - _pos.x;
	if (subX != 0)
	{
		int absX = abs(subX);
		int lenX = _rect.size.w / 2 - space;
		int signX = subX / absX;
		if (absX > lenX)
		{
			_pos.x = targetPos.x - lenX * signX;
		}
	}
	int subY = targetPos.y - _pos.y;
	if (subY != 0)
	{
		int absY = abs(subY);
		int lenY = _rect.size.h / 2 - space;
		int signY = subY / absY;
		if (absY > lenY)
		{
			_pos.y = targetPos.y - lenY * signY;
		}
	}
}

Camera::Camera(const Rect& rect) :_rect(rect)
{
	_pos = Vector2(0,0);

	// size.w == 0 ÇÕêßå¿Ç»Çµ
	_limitRect.size.w = 0;

	_follower = &Camera::NormalFollow;
}

Camera::~Camera()
{
}

void Camera::Update()
{
	for (auto itr = _targets.begin(); itr != _targets.end();)
	{
		if (*itr == nullptr)
		{
			itr = _targets.erase(itr);
		}
		else
		{
			itr++;
		}
	}

	// ÉJÉÅÉâÇÃèÍèäÇåàÇﬂÇÈ
	if (_targets.size() > 0)
	{
		(this->*_follower)();
	}

	_rect.center = Vector2Int(static_cast<int>(_pos.x), static_cast<int>(_pos.y));

	// size.w == 0 ÇÕêßå¿Ç»Çµ
	if (_limitRect.Width() != 0)
	{
		// îÕàÕêßå‰
		if (_rect.Left() < _limitRect.Left())
		{
			_rect.center.x = _limitRect.Left() + _rect.Width() / 2;
			_pos.x = static_cast<float>(_rect.center.x);
		}
		if (_rect.Right() > _limitRect.Right())
		{
			_rect.center.x = _limitRect.Right() - _rect.Width() / 2;
			_pos.x = static_cast<float>(_rect.center.x);
		}
		if (_rect.Top() < _limitRect.Top())
		{
			_rect.center.y = _limitRect.Top() + _rect.Height() / 2;
			_pos.y = static_cast<float>(_rect.center.y);
		}
		if (_rect.Botton() > _limitRect.Botton())
		{
			_rect.center.y = _limitRect.Botton() - _rect.Height() / 2;
			_pos.y = static_cast<float>(_rect.center.y);
		}
	}
}

void Camera::AddTargetActor(Actor* target)
{
	_targets.emplace_front(target);
}

void Camera::RemoveTargetActor(Actor* target)
{
	auto it = find(_targets.begin(), _targets.end(), target);
	if (it == _targets.end())
	{
		return;
	}
	_targets.erase(it);
}

void Camera::PopTargetActor()
{
	if (_targets.size() <= 0)
	{
		return;
	}
	_targets.pop_front();
}

void Camera::ClearTargetActor()
{
	_targets.clear();
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

void Camera::SetLooseFollow(bool lose)
{
	_follower = lose ? &Camera::LooseFollow : &Camera::NormalFollow;
}

const Rect& Camera::GetRect() const
{
	return _rect;
}
