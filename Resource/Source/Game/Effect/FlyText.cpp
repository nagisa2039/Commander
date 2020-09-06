#include "FlyText.h"
#include <DxLib.h>
#include <sstream>
#include "Input.h"
#include "Camera.h"
#include "Application.h"
#include "FileSystem.h"

using namespace std;

namespace
{
	constexpr float JUMP_POWER = 10.0f;
	constexpr float GRAVITY = 0.8f;
}

FlyText::FlyText(const string& str, const Vector2Int& pos, const int lifeCnt, 
	Camera& camera, const bool cameraActive, const bool critical)
	:TextEffect(_str, pos, camera, cameraActive), _anker(Anker::centerdown), _move(0, -50), _critical(critical)
{
	_moveTrack = make_unique<Track<float>>();
	_moveTrack->AddKey(0, 0.0f);
	_moveTrack->AddKey(lifeCnt, 1.0f);

	_alphaTrack = make_unique<Track<float>>();
	_alphaTrack->AddKey(0, 1.0f);
	_alphaTrack->AddKey(static_cast<uint32_t>(lifeCnt * 0.8f), 1.0f);
	_alphaTrack->AddKey(lifeCnt, 0.0f);

	if (critical)
	{
		_fontHandle = FontHandle("choplin80edge");
		_color = 0xdddd00;
	}
	else
	{
		_fontHandle = FontHandle("choplin60edge");
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

	_pos = Lerp(_pos, _pos + _move, _moveTrack->GetValue());

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

PopupText::PopupText(const std::string& str, const Vector2Int& pos, Camera& camera, const bool cameraActive, const bool critical)
	:TextEffect(str, pos, camera, cameraActive)
{
	_charInfs.reserve(str.size());

	if (critical)
	{
		_fontHandle = FontHandle("choplin80edge");
		_color = 0xdddd00;
	}
	else
	{
		_fontHandle = FontHandle("choplin60edge");
		_color = 0xffffff;
	}

	Size strSize;
	int line = 0;
	GetDrawStringSizeToHandle(&strSize.w, &strSize.h, &line, str.c_str(), str.size(), _fontHandle);

	auto GetNumSize = [](const char num, const int fontH, Size& out)
	{
		int line = 0;
		GetDrawStringSizeToHandle(&out.w, &out.h, &line, &num, 1, fontH);
	};

	Vector2Int currentCenterDown = Vector2Int(pos.x - strSize.w/2, pos.y - strSize.h/2);
	for (int i = 0; i < str.size(); ++i)
	{
		Size charSize;
		char c = str.data()[i];
		GetNumSize(c, _fontHandle, charSize);
		currentCenterDown.x += charSize.w / 2;
		_charInfs.push_back({ str.data()[i], currentCenterDown, static_cast<float>(currentCenterDown.y), 0.0f, false });
		currentCenterDown.x += charSize.w / 2;
	}
	_currentIdx = 0;
	_moveTrack = make_unique<Track_f>();
	_moveTrack->AddKey(0, 0);
	_moveTrack->AddKey(0, 15);

	_alphaTrack = make_unique<Track_f>();
	_alphaTrack->AddKey(0, 1.0f);
	_alphaTrack->AddKey(30, 1.0f);
	_alphaTrack->AddKey(60, 0.0f);
}

void PopupText::Update(const Input& input)
{
	if (_currentIdx < _charInfs.size())
	{
		_moveTrack->Update();
		if (_moveTrack->GetEnd())
		{
			_charInfs[_currentIdx].velocityY = -JUMP_POWER;
			_currentIdx++;
		}
	}

	for (int i = 0; i < _currentIdx; ++i)
	{
		if (_charInfs[i].end)continue;

		_charInfs[i].velocityY += GRAVITY;
		_charInfs[i].currentY += _charInfs[i].velocityY;
		if (_charInfs[i].currentY > _charInfs[i].startPos.y)
		{
			_charInfs[i].currentY = static_cast<float>(_charInfs[i].startPos.y);
			_charInfs[i].end = true;
		}
	}

	if (_charInfs.back().end)
	{
		_alphaTrack->Update();
		if (_alphaTrack->GetEnd())
		{
			_delete = true;
		}
	}
}

void PopupText::Draw()
{
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(255 * _alphaTrack->GetValue()));
	for (const auto& charInf : _charInfs)
	{
		DrawStringToHandle(Vector2Int(charInf.startPos.x, static_cast<int>(charInf.currentY)), 
			Anker::centerdown, _color, _fontHandle, "%c",charInf.c);
	}
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
}

TextEffect::TextEffect(const std::string& str, const Vector2Int& pos, Camera& camera, const bool cameraActive)
	:Effect(pos, camera, cameraActive), _str(str)
{
	_color = 0xffffff;
	_fontHandle = -1;
}

void TextEffect::SetFontHandle(const int handle)
{
	_fontHandle = handle;
}

void TextEffect::SetColor(const unsigned int color)
{
	_color = color;
}