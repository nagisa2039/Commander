#include "FlyText.h"
#include <DxLib.h>
#include "Input.h"
#include "Camera.h"
#include "Application.h"
#include "FileSystem.h"

using namespace std;

FlyText::FlyText(const string& str, const Vector2Int& pos, const int lifeCnt, Camera& camera, bool cameraActive, bool critical)
	:Effect(pos, camera, cameraActive), _str(str), _anker(Anker::center), _startPos(pos.ToVector2()), _move(0, -50), _critical(critical)
{
	_moveTrack = make_unique<Track<float>>();
	_moveTrack->AddKey(0, 0.0f);
	_moveTrack->AddKey(lifeCnt, 1.0f);

	_alphaTrack = make_unique<Track<float>>();
	_alphaTrack->AddKey(0, 1.0f);
	_alphaTrack->AddKey(lifeCnt * 0.8, 1.0f);
	_alphaTrack->AddKey(lifeCnt, 0.0f);

	if (critical)
	{
		_fontHandle = Application::Instance().GetFileSystem().GetFontHandle("choplin80edge");
		_color = 0xdddd00;
	}
	else
	{
		_fontHandle = Application::Instance().GetFileSystem().GetFontHandle("choplin60edge");
		_color = 0xffffff;
	}
}

FlyText::~FlyText()
{
}

void FlyText::Update(const Input& input)
{
	_moveTrack->Update();
	_alphaTrack->Update();

	_pos = Lerp(_startPos, _startPos + _move, _moveTrack->GetValue());

	if (_moveTrack->GetEnd())
	{
		_delete = true;
	}
}

void FlyText::Draw()
{
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(_alphaTrack->GetValue() * 255));
	DrawStringToHandle(_pos.ToVector2Int(), _anker, _color, _fontHandle, _str.c_str());
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
}

void FlyText::SetAnker(const Anker anker)
{
	_anker = anker;
}

void FlyText::SetMove(const Vector2& move)
{
	_move = move;
}

void FlyText::SetFontHandle(const int handle)
{
	_fontHandle = handle;
}

void FlyText::SetColor(const unsigned int color)
{
	_color = color;
}

void FlyText::SetStartPos(const Vector2& startPos)
{
	_startPos = startPos;
}
