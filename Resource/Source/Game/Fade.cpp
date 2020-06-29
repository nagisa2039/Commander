#include "Fade.h"
#include <Dxlib.h>
#include "Application.h"
#include "Geometry.h"
#include "DxLibUtility.h"

using namespace std;

Fade::Fade()
{
	_fadeTrack = make_unique<Track<float>>();
	_fadeTrack->AddKey(0, 1.0f);
	_fadeTrack->AddKey(60, 0.0f);
	_fadeColor = 0x000000;
}

Fade::~Fade()
{
}

void Fade::Update()
{
	_fadeTrack->Update();
}

void Fade::Draw()
{
	if (_fadeTrack->GetEnd())return;

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(255 * _fadeTrack->GetValue()));
	DrawBox(Vector2Int(0,0), Application::Instance().GetWindowSize().ToVector2Int(), _fadeColor);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
}

bool Fade::GetEnd() const
{
	return _fadeTrack->GetEnd();
}

void Fade::StartFadeIn(const unsigned color)
{
	_fadeTrack->SetReverse(false);
	_fadeTrack->Reset();
	_fadeColor = color;
}

void Fade::StartFadeOut(const unsigned color)
{
	_fadeTrack->SetReverse(true);
	_fadeTrack->Reset();
	_fadeColor = color;
}