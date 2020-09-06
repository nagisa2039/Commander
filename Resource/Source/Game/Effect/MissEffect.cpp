#include "MissEffect.h"
#include <Dxlib.h>
#include "DxLibUtility.h"
#include "Application.h"
#include "FileSystem.h"

MissEffect::MissEffect(const Vector2Int& pos, Camera& camera): Effect(pos, camera)
{
	_animPosYTrack = std::make_unique<Track<float>>();
	_animPosYTrack->AddKey(0, 0.0f);
	_animPosYTrack->AddKey(10, -50.0f);
	_animPosYTrack->AddKey(30, 0.0f);
}

MissEffect::~MissEffect()
{
}

void MissEffect::Update(const Input& input)
{
	_animPosYTrack->Update();
	if (_animPosYTrack->GetEnd())
	{
		_delete = true;
	}
}

void MissEffect::Draw()
{
	const char* str = "MISSÅI";
	auto fontH = FontHandle("choplin30edge");
	Size strSize;
	int line;
	GetDrawFormatStringSizeToHandle(&strSize.w, &strSize.h, &line, fontH, str);
	auto drawPos = GetDrawPos(_pos.ToVector2Int() + Vector2Int(0, static_cast<int>(_animPosYTrack->GetValue())), strSize, Anker::center);
	DrawFormatStringToHandle(drawPos.x, drawPos.y, 0xffffff, fontH, str);
}
