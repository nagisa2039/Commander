#include "FlyText.h"
#include <DxLib.h>
#include "Input.h"
#include "Camera.h"
#include "Application.h"
#include "FileSystem.h"

using namespace std;

FlyText::FlyText(const string& str, const Vector2Int& pos, const int lifeCnt)
	:Effect(pos), _str(str), _lifeCntMax(lifeCnt), _lifeCnt(lifeCnt), 
	_anker(Anker::center), _startPos(pos.ToVector2()), _move(0, -50)
{
	Application::Instance().GetFileSystem()->FontInit("Resource/Font/Choplin.ttf", "Choplin", "choplin", 40, 1, true, true);
	_fontHandle = Application::Instance().GetFileSystem()->GetFontHandle("choplin");

	_color = 0xffffff;
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

void FlyText::Draw(const Camera& camera)
{
	auto offset = camera.GetCameraOffset();

	auto len = strlen(_str.c_str());
	Size size;
	int lineCnt = 0;
	GetDrawStringSizeToHandle(&size.w, &size.h, &lineCnt, _str.c_str(), len, _fontHandle);

	auto drawPos = GetDrawPos(_pos.ToVector2Int(), size, _anker);

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, _lifeCnt-- / static_cast<float>(_lifeCntMax) * 255);
	DrawFormatStringToHandle(drawPos, _color, _fontHandle, _str.c_str());
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