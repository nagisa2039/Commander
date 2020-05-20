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

	if (_targets.size() > 0)
	{
		// ÉJÉÅÉâÇÃèÍèäÇåàÇﬂÇÈ
		Vector2 pos2D = { 0.0f, 0.0f };
		/*for (auto& target : _targets)
		{
			pos2D += target->GetCenterPos();
		}
		Vector3 targetPos = Vector3(pos2D.x, pos2D.y, 0) / _targets.size();*/
		pos2D = (*_targets.begin())->GetCenterPos();
		_pos = Lerp(_pos, Vector3(pos2D.x, pos2D.y, 0), 0.1f);
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
