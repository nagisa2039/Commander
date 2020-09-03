#include <Dxlib.h>
#include "CutIn.h"
#include "Application.h"
#include "DataBase.h"

using namespace std;

namespace
{
	constexpr int BAND_SIZE_H = 15;
	constexpr int SRC_RECT_SIZE = 32;
	constexpr int SRC_CUT_HEIGHT = 24;
}

CutIn::CutIn(const CharactorType type, const Team team, const Dir dir)
	:_charactorType(type), _team(team), _dir(dir)
{
	_animTrack = make_unique<Track_f>();
	_animTrack->AddKey(0,  0.0f);
	_animTrack->AddKey(15, 1.0f);
	_animTrack->AddKey(60, 1.0f);
	_animTrack->AddKey(75,0.0f);
	_moveTrack = make_unique<Track_f>();
	_moveTrack->AddKey(0, 0.0f);
	_moveTrack->AddKey(15, 1.0f);
	_moveTrack->AddKey(60, 1.0f);
	_moveTrack->AddKey(75, 2.0f);

	_graphH = Application::Instance().GetDataBase().GetCharactorImageHandle(type, team);
}

CutIn::~CutIn()
{
}

void CutIn::Update()
{
	_animTrack->Update();
	_moveTrack->Update();
}

void CutIn::Draw()
{
	auto& wsize = Application::Instance().GetWindowSize();
	Size charSize(200, 200);
	auto center = wsize.ToVector2Int() * 0.5f;
	float animValue = _animTrack->GetValue();
	Rect catinRect(center, Size(wsize.w, Lerp(0, charSize.h, animValue)+ BAND_SIZE_H * 2));
	catinRect.Draw(0x000000);
	catinRect.size.h -= BAND_SIZE_H * 2;
	catinRect.Draw(0xffff00);
	auto moveValue = _moveTrack->GetValue();
	Rect charRect(Vector2Int(Lerp(_dir == Dir::left ? -charSize.w : wsize.w + charSize.w, center.x, moveValue), center.y), 
		Size(charSize.w, Lerp(0, charSize.h, animValue)));
	int srcY = (_dir == Dir::left ? SRC_RECT_SIZE*2 : SRC_RECT_SIZE);
	int h = Lerp(0, SRC_CUT_HEIGHT, animValue);

	SetDrawMode(DX_DRAWMODE_NEAREST);
	charRect.DrawRectGraph(
		Vector2Int(SRC_RECT_SIZE + SRC_RECT_SIZE/2 - (SRC_CUT_HEIGHT/2), srcY + SRC_CUT_HEIGHT /2 - h/2), 
		Size(SRC_CUT_HEIGHT, h), _graphH);
	SetDrawMode(DX_DRAWMODE_BILINEAR);
}

bool CutIn::GetEnd() const
{
	return _animTrack->GetEnd();
}
