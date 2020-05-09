#include "Effect.h"
#include "Animator.h"
#include "Input.h"
#include "Camera.h"

using namespace std;

Effect::Effect(const Vector2Int& pos, Camera& camera, bool cameraActive):Actor(camera), _cameraActive(cameraActive)
{
	_pos = pos.ToVector2();
	_animator = make_shared<Animator>();
	_size = Size();
}

Effect::~Effect()
{
}

void Effect::Update(const Input& input)
{
	if (_delete)
	{
		return;
	}
	_animator->Update();
	if (_animator->GetAnimEnd())
	{
		_delete = true;
	}
}

void Effect::Draw()
{
	auto offset = _cameraActive ? _camera.GetCameraOffset() : Vector2Int(0,0);
	_animator->Draw(offset + _pos.ToVector2Int() - _size.ToVector2Int()*0.5f, _size);
}
