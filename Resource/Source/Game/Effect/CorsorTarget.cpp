#include "CorsorTarget.h"
#include "Animator.h"
#include <Dxlib.h>
#include "DxLibUtility.h"
#include "Camera.h"

CorsorTarget::CorsorTarget(const Vector2Int& centerPos, Camera& camera, bool cameraActive, const Size& chipSize): Effect(centerPos, camera, cameraActive), _chipSize(chipSize)
{
	_animator->SetImage("Resource/Image/MapChip/cursor.png");
	_exRateTrack = std::make_unique<Track<float>>();
	_exRateTrack->AddKey(0, 1.0f);
	_exRateTrack->AddKey(15, 1.0f);
	_exRateTrack->AddKey(16, 0.8f);
	_exRateTrack->AddKey(30, 0.8f);
	_exRateTrack->AddKey(31, 1.0f);
	_exRateTrack->AddKey(45, 1.0f);
	_exRateTrack->AddKey(46, 0.8f);
	_exRateTrack->AddKey(60, 0.8f);
}

CorsorTarget::~CorsorTarget()
{
}

void CorsorTarget::Update(const Input& input)
{
	_exRateTrack->Update();
	if (_exRateTrack->GetEnd())
	{
		_delete = true;
	}
}

void CorsorTarget::Draw()
{
	Vector2Int offset = _cameraActive ? _camera.GetCameraOffset() : Vector2Int{ 0, 0 };
	auto center = offset + _pos.ToVector2Int();
	Rect drawRect{ center, _chipSize * _exRateTrack->GetValue() };

	DrawExtendGraph(drawRect.Left(), drawRect.Top(), drawRect.Right(), drawRect.Botton(), _animator->GetImageH(), true);
}

void CorsorTarget::Reset()
{
	Effect::Reset();
	_exRateTrack->Reset();
}
