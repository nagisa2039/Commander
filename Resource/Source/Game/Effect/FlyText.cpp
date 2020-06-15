#include "FlyText.h"
#include <DxLib.h>
#include "Input.h"
#include "Camera.h"
#include "Application.h"
#include "FileSystem.h"

using namespace std;

FlyText::FlyText(const string& str, const Vector2Int& pos, const int lifeCnt, Camera& camera, bool cameraActive, bool critical)
	:Effect(pos, camera, cameraActive), _str(str), _lifeCntMax(lifeCnt), _lifeCnt(lifeCnt),
	_anker(Anker::center), _startPos(pos.ToVector2()), _move(0, -50), _critical(critical)
{
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
	float p = 1.0f - (_lifeCnt-- / static_cast<float>(_lifeCntMax));
	_pos = Lerp(_startPos, _startPos + _move, p);

	if (p > 1.0f)
	{
		_delete = true;
	}
}

void FlyText::Draw()
{
	auto offset = _cameraActive ? _camera.GetCameraOffset() : Vector2Int(0, 0);
	auto len = strlen(_str.c_str());
	Size size;
	int lineCnt = 0;
	GetDrawStringSizeToHandle(&size.w, &size.h, &lineCnt, _str.c_str(), len, _fontHandle);

	auto drawPos = offset + GetDrawPos(_pos.ToVector2Int(), size, _anker);

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, _lifeCnt-- / static_cast<float>(_lifeCntMax) * 255);
	DrawFormatStringToHandle(drawPos.x, drawPos.y, _color, _fontHandle, _str.c_str());
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
