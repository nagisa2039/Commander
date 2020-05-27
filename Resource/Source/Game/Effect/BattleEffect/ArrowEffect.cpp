#include "ArrowEffect.h"
#include "../Game/Animator.h"
#include <dxlib.h>
#include "DxLibUtility.h"
#include "Camera.h"

ArrowEffect::ArrowEffect(const Vector2Int& pos, const Vector2Int& targetPos, std::vector<std::shared_ptr<Effect>>& effects, Camera& camera)
	: BattleEffect(pos, targetPos, effects, camera)
{
	_delete = false;
	_animator->SetImage("Resource/Image/Effect/arrow.png");
	GetGraphSize(_animator->GetImageH(), _size);
	_animator->AddAnimDiv("normal", Rect(_size.ToVector2Int() * 0.5, _size), 1, 3, true, false);
	_animator->ChangeAnim("normal");

	_moveTrack = std::make_shared<Track<Vector2Int>>();
	_moveTrack->ClearKey();
	_moveTrack->AddKey(0, pos);
	if ((targetPos - pos).x > 0)
	{
		_moveTrack->AddKey(30, targetPos - Vector2Int(_animator->GetImageSize().w/2, 0));
	}
	else
	{
		_moveTrack->AddKey(30, targetPos + Vector2Int(_animator->GetImageSize().w/2, 0));
		_animator->SetReverseY(true);
	}
}

ArrowEffect::~ArrowEffect()
{
}

void ArrowEffect::Update(const Input& input)
{
	if (_delete)
	{
		return;
	}

	_moveTrack->Update();
	if (_moveTrack->GetEnd())
	{
		_delete = true;
	}
}

void ArrowEffect::Draw()
{
	auto offset = _cameraActive ? _camera.GetCameraOffset() : Vector2Int(0, 0);
	_animator->Draw(offset + _moveTrack->GetValue() - (_size*0.5f).ToVector2Int(), _size);
}
