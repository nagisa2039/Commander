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
	teamInfs[static_cast<size_t>(Team::player)] = TeamInf("PLAYER");
	teamInfs[static_cast<size_t>(Team::enemy)]	= TeamInf("ENEMY");

	_currentTeam = Team::player;
	_animEnd = false;

	_frame = 0;

	_exrateTL.ClearKey();
	_exrateTL.AddKey(make_pair(0,	2));
	_exrateTL.AddKey(make_pair(15,	1));
	_exrateTL.AddKey(make_pair(105, 1));
	_exrateTL.AddKey(make_pair(120, 2));

	_alphaTL.ClearKey();
	_alphaTL.AddKey(make_pair(0, 0));
	_alphaTL.AddKey(make_pair(15, 255));
	_alphaTL.AddKey(make_pair(105, 255));
	_alphaTL.AddKey(make_pair(120, 0));
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
	auto blurHandle = Application::Instance().GetFileSystem()->GetImageHandle("Resource/Image/Battle/magicCircleBlur.png");

	auto teamInf = teamInfs[static_cast<size_t>(_currentTeam)];

	// 魔法陣の描画
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);

	// ぼかし
	auto teamColor = GetTeamColor(_currentTeam);
	SetDrawBright((teamColor << 8) >> 24, (teamColor << 16) >> 24, (teamColor << 24) >> 24);
	DrawRotaGraph(wsize.w / 2, wsize.h / 2, exrate,
	_frame * 5.0 * DX_PI / 180.0f, blurHandle, true);
	SetDrawBright(255,255,255);

	// オリジナル
	DrawRotaGraph(wsize.w / 2, wsize.h / 2, exrate,
		_frame * 5.0 * DX_PI / 180.0f, gHangle, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);

	// 名前表示
	char str[20];
	sprintf_s(str, 20, "%s%s", teamInf.name, " TURN");
	Size strSize;
	int lineCnt;
	auto fontHandle = Application::Instance().GetFileSystem()->GetFontHandle("choplin100");
	GetDrawFormatStringSizeToHandle(&strSize.w, &strSize.h, &lineCnt, fontHandle, str);
	auto nameDrawPos = GetDrawPos(wsize.ToVector2Int() * 0.5f, strSize, Anker::center);
	DrawFormatStringToHandle(nameDrawPos.x, nameDrawPos.y, teamColor, fontHandle, str);
}

void TurnChangeAnim::TurnStart(const Team team)
{
	_animEnd = false;
	_currentTeam = team;
	_frame = 0;
}

bool TurnChangeAnim::GetAnimEnd() const
{
	return _animEnd;
}
