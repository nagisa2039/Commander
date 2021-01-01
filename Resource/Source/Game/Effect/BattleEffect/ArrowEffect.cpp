#include "ArrowEffect.h"
#include "../Game/Animator.h"
#include <dxlib.h>
#include "DxLibUtility.h"
#include "Camera.h"
#include "Charactor.h"
#include "BattleCharactor.h"
#include "FileSystem.h"
#include <vector>
#include <memory>

ArrowEffect::ArrowEffect(BattleCharactor& self, BattleCharactor& target, std::vector<std::shared_ptr<Effect>>& effects, 
	bool critical, Camera& camera)
	: BattleEffect(self, target, effects, critical, camera)
{
	_delete = false;
	_animator->SetImage("Resource/Image/Effect/arrow.png");
	GetGraphSize(_animator->GetImageH(), _size);
	_animator->AddAnimDiv("normal", Rect{ _size.ToVector2Int() * 0.5, _size }, 1, 3, true, false);
	_animator->ChangeAnim("normal");

	auto startPos = self.GetCenterPos();
	auto targetPos = target.GetCenterPos();

	_moveTrack = std::make_shared<Track<Vector2Int>>();
	_moveTrack->ClearKey();
	_moveTrack->AddKey(0, startPos);
	if ((targetPos - startPos).x > 0)
	{
		_moveTrack->AddKey(30, targetPos - Vector2Int{ _animator->GetImageSize().w / 2, 0 });
	}
	else
	{
		_moveTrack->AddKey(30, targetPos + Vector2Int{ _animator->GetImageSize().w / 2, 0 });
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
		_self.SetGivenDamage(AddDamage());
		_delete = true;
	}
}

void ArrowEffect::Draw()
{
	auto offset = _cameraActive ? _camera.GetCameraOffset() : Vector2Int{ 0, 0 };
	_animator->Draw(offset + _moveTrack->GetValue() - (_size*0.5f).ToVector2Int(), _size);
}
