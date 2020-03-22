#include "TurnChangeAnim.h"
#include "Input.h"
#include "Camera.h"
#include "Application.h"
#include "FileSystem.h"
#include "DxLibUtility.h"
#include <DxLib.h>

using namespace std;

TurnChangeAnim::TurnChangeAnim()
{
	teamInfs[static_cast<size_t>(Team::player)] = TeamInf("PLAYER",	0x0000ff);
	teamInfs[static_cast<size_t>(Team::enemy)]	= TeamInf("ENEMY",	0x0000ff);

	_currentTeam = Team::player;
	_animEnd = false;

	_frame = 0;

	_exrateTL.ClearKey();
	_exrateTL.AddKey(make_pair(0,	2));
	_exrateTL.AddKey(make_pair(30,	1));
	_exrateTL.AddKey(make_pair(210, 1));
	_exrateTL.AddKey(make_pair(240, 2));

	_alphaTL.ClearKey();
	_alphaTL.AddKey(make_pair(0, 0));
	_alphaTL.AddKey(make_pair(30, 255));
	_alphaTL.AddKey(make_pair(210, 255));
	_alphaTL.AddKey(make_pair(240, 0));
}

TurnChangeAnim::~TurnChangeAnim()
{
}

void TurnChangeAnim::Update(const Input& input)
{
	if (_animEnd) return;

	_frame++;
}

void TurnChangeAnim::Draw(const Camera& camera)
{
	auto alpha = _alphaTL.GetValue(_frame);
	auto exrate = _exrateTL.GetValue(_frame);
	_animEnd = _alphaTL.GetEnd();

	if (_animEnd) return;

	auto wsize = Application::Instance().GetWindowSize();
	auto gHangle = Application::Instance().GetFileSystem()->GetImageHandle("Resource/Image/Battle/magicCircle.png");

	auto teamInf = teamInfs[static_cast<size_t>(_currentTeam)];

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
	DrawRotaGraph(wsize.w / 2, wsize.h / 2, exrate,
		_frame * 5.0 * DX_PI / 180.0f, gHangle, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);

	// –¼‘O•\Ž¦
	char str[20];
	sprintf_s(str, 20, "%s%s", teamInf.name, " TURN");
	Size strSize;
	int lineCnt;
	auto fontHandle = Application::Instance().GetFileSystem()->GetFontHandle("choplin100");
	GetDrawFormatStringSizeToHandle(&strSize.w, &strSize.h, &lineCnt, fontHandle, str);
	auto nameDrawPos = GetDrawPos(wsize.ToVector2Int() * 0.5f, strSize, Anker::center);
	DrawFormatStringToHandle(nameDrawPos.x, nameDrawPos.y, teamInf.color, fontHandle, str);
}

void TurnChangeAnim::TurnStart(const Team team)
{
	_animEnd = false;
	_currentTeam = team;
}

bool TurnChangeAnim::GetAnimEnd() const
{
	return _animEnd;
}
