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

	_timeLine.clear();
	_currentVal = Key(0, 0, 0);

	_timeLine.emplace_back(Key(0, 2, 0));
	_timeLine.emplace_back(Key(30, 1, 255));
	_timeLine.emplace_back(Key(210, 1, 255));
	_timeLine.emplace_back(Key(240, 2, 0));
}

TurnChangeAnim::~TurnChangeAnim()
{
}

void TurnChangeAnim::Update(const Input& input)
{
	if (_animEnd) return;

	_currentVal.frame++;
	auto frame = _currentVal.frame;
	auto it = find_if(_timeLine.rbegin(), _timeLine.rend(), [frame](const Key key)
	{
		return frame >= key.frame;
	});

	// 指定フレームのアニメーションが無いのでとばす
	if (it == _timeLine.rend())
	{
		_animEnd = true;
		return;
	}

	auto currnet = it;
	auto next = it.base();

	if (_timeLine.end() == next)
	{
		_currentVal = *currnet;
		_animEnd = true;
		return;
	}
	
	auto parsent = (_currentVal.frame - currnet->frame) / (float)(next->frame - currnet->frame);
	_currentVal.exrate = Lerp(currnet->exrate, next->exrate, parsent);
	_currentVal.alpha = Lerp(currnet->alpha, next->alpha, parsent);
}

void TurnChangeAnim::Draw(const Camera& camera)
{
	if (_animEnd) return;

	auto wsize = Application::Instance().GetWindowSize();
	auto gHangle = Application::Instance().GetFileSystem()->GetImageHandle("Resource/Image/Battle/magicCircle.png");

	auto teamInf = teamInfs[static_cast<size_t>(_currentTeam)];

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, _currentVal.alpha);
	DrawRotaGraph(wsize.w / 2, wsize.h / 2, _currentVal.exrate,
		_currentVal.frame * 5.0 * DX_PI / 180.0f, gHangle, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);

	// 名前表示
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
