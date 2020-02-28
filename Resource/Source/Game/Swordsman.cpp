#include "Swordsman.h"
#include "MapCtrl.h"
#include "../Utility/DxLibUtility.h"
#include "Camera.h"
#include "Animator.h"
#include "../Scene/SceneController.h"
#include "SlashingEffect.h"
#include "Application.h"
#include <DxLib.h>
#include "FileSystem.h"

using namespace std;

Swordsman::Swordsman(const Vector2Int& mapPos, const Team team, MapCtrl& mapCtrl, SceneController& ctrl, 
	std::vector<std::shared_ptr<Effect>>& effects)
	:Charactor(mapPos, team, mapCtrl, ctrl, effects)
{
	_battleC = make_shared<SwordBC>(*this);

	const Size divSize = Size(32,32);
	_animator->SetImage("Resource/Image/Charactor/charactor.png");

	int cnt = 0;
	auto nextRectCenterOffset = [&](std::vector<Rect>& animRectVec, int cnt)
	{
		for (auto& rect : animRectVec)
		{
			rect.center.y += divSize.h;
		}
	};

	std::vector<Rect> animRectVec;
	animRectVec.emplace_back(Rect(Vector2Int(16, 16), divSize));
	animRectVec.emplace_back(Rect(Vector2Int(16 + divSize.w * 2, 16), divSize));

	_animator->AddAnim("DownWalk", animRectVec, 30, true);
	nextRectCenterOffset(animRectVec, ++cnt);
	_animator->AddAnim("LeftWalk", animRectVec, 30, true);
	nextRectCenterOffset(animRectVec, ++cnt);
	_animator->AddAnim("RightWalk", animRectVec, 30, true);
	nextRectCenterOffset(animRectVec, ++cnt);
	_animator->AddAnim("UpWalk", animRectVec, 30, true);

	_animator->ChangeAnim("LeftWalk");

	_dirTable[Dir::left].animName = "LeftWalk";
	_dirTable[Dir::right].animName = "RightWalk";
	_dirTable[Dir::up].animName = "UpWalk";
	_dirTable[Dir::down].animName = "DownWalk";

	_dir = Dir::down;

	_status = Status(1, 100, 10, 5, 5, 5, 5, 5);
	_startStatus = _status;
}

Swordsman::~Swordsman()
{
}

void Swordsman::Update(const Input& input)
{
	Move();

	_animator->ChangeAnim(_dirTable[_dir].animName);
	_animator->Update();
}

void Swordsman::Draw(const Camera& camera)
{
	auto offset = camera.GetCameraOffset();
	auto chipSize = _mapCtrl.GetChipSize().ToVector2Int();

	DrawMovableMass(camera);

	auto imgSize = _animator->GetAnimRect().size.ToVector2Int();
	_animator->Draw(offset + _pos.ToVector2Int(), _mapCtrl.GetChipSize());

	auto circleOffset = Vector2Int(0, -chipSize.y/2);
	DrawCircle(circleOffset + offset + _pos.ToVector2Int() + chipSize * 0.5, chipSize.x / 4, GetTeamColor(), true);
}

SwordBC::SwordBC(Charactor& charactor): BattleCharactor(charactor)
{
	const Size divSize = Size(32, 32);
	_animator->SetImage("Resource/Image/Charactor/charactor.png"); 
	
	int cnt = 0;
	auto nextRectCenterOffset = [&](std::vector<Rect>& animRectVec, int cnt)
	{
		for (auto& rect : animRectVec)
		{
			rect.center.y += divSize.h;
		}
	};

	std::vector<Rect> animRectVec;
	animRectVec.clear();

	animRectVec.emplace_back(Rect(Vector2Int(16,				 16 + 32 * 1), divSize));
	animRectVec.emplace_back(Rect(Vector2Int(16 + divSize.w * 2, 16 + 32 * 1), divSize));
	_animator->AddAnim("LeftWalk", animRectVec, 30, true);
	animRectVec.clear();

	animRectVec.emplace_back(Rect(Vector2Int(16,				 16 + 32 * 2), divSize));
	animRectVec.emplace_back(Rect(Vector2Int(16 + divSize.w * 2, 16 + 32 * 2), divSize));
	_animator->AddAnim("RightWalk", animRectVec, 30, true);

}

SwordBC::~SwordBC()
{
}

void SwordBC::Update(BattleScene& battleScene)
{
}

void SwordBC::UIDraw()
{
	auto wsize = Application::Instance().GetWindowSize();
	auto status = _selfChar.GetStatus();
	auto targetStatus = _targetChar->GetSelfCharacotr().GetStatus();
	auto teamColor = GetTeamColor(_selfChar.GetTeam());
	auto fontHandle = Application::Instance().GetFileSystem()->GetFontHandle("choplin");

	// UI‚Ì¶ã‚ÌÀ•W
	Vector2Int dirDownPos;
	Vector2Int paramDrawPos;
	Size paramUISize(_uiSize.w / 3, 120);

	if (_dir == Dir::left)
	{
		dirDownPos = GetDrawPos(Vector2Int(0, wsize.h), _uiSize, Anker::leftdown);
		paramDrawPos = GetDrawPos(Vector2Int(0, wsize.h - _uiSize.h/2), paramUISize, Anker::leftdown);
	}
	else
	{
		dirDownPos = GetDrawPos(wsize.ToVector2Int(), _uiSize, Anker::rightdown);
		paramDrawPos = GetDrawPos(wsize.ToVector2Int() - Vector2Int(0, _uiSize.h/2), paramUISize, Anker::rightdown);
	}
	// ‰æ–Ê‰ºUI‚Ì—Ìˆæ•`‰æ
	DrawBox(dirDownPos, dirDownPos + _uiSize.ToVector2Int(), teamColor, true);

	// UŒ‚—Í, –½’†—¦, •KŽE, ‚Ì•`‰æ
	Rect _paramRect(paramDrawPos + paramUISize.ToVector2Int()*0.5, paramUISize);
	_paramRect.Draw(teamColor);
	_paramRect.Draw(0xffffff, false);
	auto drawParam = [&](const int itemNum, const int fontH, const unsigned int color, const char* string, const int num)
	{
		Size strSize;
		int lineCnt;
		GetDrawFormatStringSizeToHandle(&strSize.w, &strSize.h, &lineCnt, fontH, "%d", num);
		Vector2Int paramNumDrawPos = GetDrawPos(Vector2Int(_paramRect.Right(), _paramRect.Top()), strSize, Anker::rightup);
		DrawFormatStringToHandle(paramDrawPos.x + 10,		paramDrawPos.y + 40 * itemNum,		color, fontH, string);
		DrawFormatStringToHandle(paramNumDrawPos.x - 10,	paramNumDrawPos.y + 40 * itemNum,	color, fontH, "%d", num);
	};

	int itemNum = 0;
	// UŒ‚—Í
	drawParam(itemNum++, fontHandle, 0xaaaaaa, "ATK", status.GetDamage(targetStatus));
	// –½’†
	drawParam(itemNum++, fontHandle, 0xaaaaaa, "HIT", 100);
	// •KŽE
	drawParam(itemNum++, fontHandle, 0xaaaaaa, "CRT", 100);

	// HP‚Ì”’l•\Ž¦
	auto hpDrawPos = dirDownPos + Vector2Int(20, 20) + Vector2Int(0, _uiSize.h/2);
	DrawFormatStringToHandle(hpDrawPos.x, hpDrawPos.y, 0xaaaaaa, fontHandle, "%d", status.health);
}

void SwordBC::SetDir(const Dir dir)
{
	_dir = dir;

	if (dir == Dir::right)
	{
		_animator->ChangeAnim("LeftWalk");
	}
	else
	{
		_animator->ChangeAnim("RightWalk");
	}
}

std::shared_ptr<Effect> SwordBC::CreateAttackEffect(const Vector2Int& effectPos)
{
	return make_shared<SlashingEffect>(effectPos);
}
